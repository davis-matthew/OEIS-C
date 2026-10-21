/*
 * A064916  a(n) = n/lpf(n) + lpf(n) - 1, where lpf = A020639 = least prime
 *          factor.
 * https://oeis.org/A064916
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / lpf(n) + lpf(n) - 1, where lpf(n) is the least prime
 *            factor of n. * The formula is implemented directly by computing
 *            the least prime factor using trial division
 * Range:     u8 n <= 256, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 2, so for n < 2 the value is not defined; return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Find the least prime factor (lpf). */
    uint64_t p = 0;
    if (n % 2ULL == 0) {
        p = 2ULL;
    } else {
        for (uint64_t i = 3; i * i <= n; i += 2) {
            if (n % i == 0) {
                p = i;
                break;
            }
        }
        if (p == 0)          /* n is prime */
            p = n;
    }

    /* Compute a(n) = n/p + p - 1.  The result never exceeds n, so it
     * fits into idx_t and can safely be cast to seq_t. */
    uint64_t result = (n / p) + p - 1;
    return (seq_t)result;
}
