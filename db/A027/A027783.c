/*
 * A027783  a(n) = 5*(n+1)*binomial(n+2,10).
 * https://oeis.org/A027783
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n+1)*binomial(n+2,10)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 27, u64 n <= 194,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    const idx_t k = 10;               /* fixed for binomial */
    seq_t binom = 1;

    /* Compute C(n+2,10) multiplicatively: C(N,i) = C(N,i-1)*(N-i+1)/i */
    for (idx_t i = 1; i <= k; ++i) {
        binom = binom * (seq_t)(n + 2 - k + i) / (seq_t)i;
    }

    return 5 * (seq_t)(n + 1) * binom;
}
