/*
 * A069528  Smallest multiple of n with digit sum = 9, or 0 if no such number
 *          exists (e.g., a(11k) = 0).
 * https://oeis.org/A069528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest multiple of n with digit sum 9, or 0 if none *
 *            This implementation uses the following observations: * - If n is
 *            a multiple of 11, 101, 271, 999, or 4649, a(n) = 0. * - Let g =
 *            gcd(n, 9). Then any multiple of n that has digit sum 9 * must be
 *            of the form n * k where k is a multiple...
 * Range:     u8 n <= 28, u16 n <= 121, u32 n <= 988, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum.h"

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Exceptions where no such multiple exists */
    if (n % 11 == 0 || n % 101 == 0 || n % 271 == 0 ||
        n % 999 == 0 || n % 4649 == 0)
        return 0;

    /* Determine the step size: 9/gcd(n,9) */
    idx_t step;
    if (n % 9 == 0)         /* gcd = 9 */
        step = 1;
    else if (n % 3 == 0)    /* gcd = 3 */
        step = 3;
    else                    /* gcd = 1 */
        step = 9;

    /* Search for the smallest k multiple of step with digit sum 9 */
    idx_t k = step;
    while (1) {
        seq_t prod = (seq_t)n * (seq_t)k;
        if (digit_sum(prod) == 9)
            return prod;
        k += step;
    }
}
