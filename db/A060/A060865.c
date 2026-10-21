/*
 * A060865  a(n) is the exact power of 2 that divides the n-th Fibonacci
 *          number (A000045).
 * https://oeis.org/A060865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n % 3 != 0 a(n) = 2 if n = 3 * (odd) a(n) = 2^(k+2)
 *            if n = 3 * 2^k * (odd), k > 0 where k = v₂(n/3) (the exponent of
 *            2 in n/3)
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool type, not actually used */

seq_t a(idx_t n)
{
    /* if n is not divisible by 3, F(n) is odd → power of 2 is 1 */
    if ((n % 3u) != 0u)
        return 1u;

    /* n is divisible by 3; factor out powers of 2 from n/3 */
    idx_t m = n / 3u;
    idx_t k = 0u;
    while ((m & 1u) == 0u) {   /* while m is even */
        m >>= 1u;
        ++k;
    }

    /* if k == 0 (n = 3 * odd) → 2^1 = 2 */
    if (k == 0u)
        return 2u;

    /* otherwise exponent is k + 2  → 2^(k+2) */
    return ((seq_t)1 << (k + 2u));
}
