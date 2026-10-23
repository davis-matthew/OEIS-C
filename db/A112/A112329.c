/*
 * A112329  Number of divisors of n if n odd, number of divisors of n/4 if n
 *          divisible by 4, otherwise 0.
 * https://oeis.org/A112329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = d(n) if n is odd, a(n) = d(n/4) if 4 divides n, otherwise
 *            0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided utility count_divisors */

#include "../utils/count_divisors.h"

seq_t a(idx_t n)
{
    /* odd n: number of divisors of n */
    if (n & 1) {
        return count_divisors(n);
    }
    /* n divisible by 4: number of divisors of n/4 */
    if ((n & 3) == 0) {
        return count_divisors(n >> 2);   /* n / 4 */
    }
    /* n = 2 mod 4: result is 0 */
    return 0;
}
