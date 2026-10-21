/*
 * A056488  Number of periodic palindromes using a maximum of six different
 *          symbols.
 * https://oeis.org/A056488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^((n+1)/2) for n odd * a(n) = 6^(n/2-1) * 21 for n even
 *            (n ≥ 2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)           /* sequence starts at n = 1 */
        return (seq_t)0;

    if (n & 1U) {        /* odd n */
        idx_t exp = (n + 1) >> 1;   /* (n+1)/2 */
        seq_t res = 1;
        for (idx_t i = 0; i < exp; ++i)
            res *= (seq_t)6;
        return res;
    } else {              /* even n, n >= 2 */
        idx_t exp = n >> 1;          /* n/2 */
        seq_t pow6 = 1;
        /* compute 6^(exp-1) */
        if (exp > 0) {
            for (idx_t i = 0; i < exp - 1; ++i)
                pow6 *= (seq_t)6;
            /* a(n) = pow6 * 21 = 6^(exp-1) * 21 */
            return pow6 * (seq_t)21;
        }
        return (seq_t)0;   /* unreachable for n >= 1, but keeps compiler happy */
    }
}
