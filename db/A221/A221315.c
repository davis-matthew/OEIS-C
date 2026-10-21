/*
 * A221315  Number of nonnegative integer arrays of length n summing to n
 *          without equal adjacent values modulo 2.
 * https://oeis.org/A221315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n % 4 == 2 a(n) = 2 * binom(n/4 + n - 1, n - 1) if n
 *            % 4 == 0 a(n) = binom((n - 1)/4 + n - 1, n - 1) if n % 4 == 1
 *            a(n) = binom((n + 1)/4 + n - 1, n - 1) if n % 4 == 3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 74, u128 n <= 142,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, but guard for safety */

    idx_t rem = n & 3;           /* n % 4 */
    seq_t result = 0;

    if (rem == 2) {
        /* a(n) = 0 for n ≡ 2 (mod 4) */
        result = 0;
    } else if (rem == 0) {
        /* a(n) = 2 * binom(n/4 + n - 1, n - 1) */
        idx_t t = n >> 2;                      /* n / 4 */
        idx_t N = t + n - 1;                   /* n/4 + n - 1 */
        result = binom(N, n - 1);
        result = result * 2;
    } else if (rem == 1) {
        /* a(n) = binom((n-1)/4 + n - 1, n - 1) */
        idx_t t = (n - 1) >> 2;                /* (n-1) / 4 */
        idx_t N = t + n - 1;                   /* (n-1)/4 + n - 1 */
        result = binom(N, n - 1);
    } else { /* rem == 3 */
        /* a(n) = binom((n+1)/4 + n - 1, n - 1) */
        idx_t t = (n + 1) >> 2;                /* (n+1) / 4 */
        idx_t N = t + n - 1;                   /* (n+1)/4 + n - 1 */
        result = binom(N, n - 1);
    }

    return result;
}
