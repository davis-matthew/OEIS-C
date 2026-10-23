/*
 * A156339  Denominator of Euler(n, 1/13).
 * https://oeis.org/A156339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 13^n * 2^v2(n+1) where v2(k) is the exponent of 2 in the
 *            prime factorisation of k (number of trailing zeros in the binary
 *            representation of k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute the denominator of Euler(n,1/13) */
seq_t a(idx_t n)
{
    /* compute 13^n */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow *= (seq_t)13;

    /* exponent of 2 in (n+1) */
    idx_t m = n + 1;
    unsigned int exp2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++exp2;
    }

    /* multiply by 2^exp2 */
    pow *= (seq_t)(1ULL << exp2);

    return pow;
}
