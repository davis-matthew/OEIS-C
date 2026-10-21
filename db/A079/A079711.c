/*
 * A079711  C(2n,n)^(2n).
 * https://oeis.org/A079711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2n,n)^(2n) (central binomial coefficient to the
 *            2n‑th power)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = C(2n,n)^(2n) for n >= 1 */
seq_t a(idx_t n)
{
    /* compute C(2n,n) multiplicatively */
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = (seq_t)(n + i);   /* n+i fits in uint64_t for all tested n */
        seq_t den = (seq_t)i;
        binom = (binom * num) / den;  /* division is exact at each step */
    }

    /* binary exponentiation of binom to exponent 2*n */
    idx_t exp = 2 * n;
    seq_t result = 1;
    seq_t base   = binom;

    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;   /* avoid one extra square when exp becomes 0 */
    }
    return result;
}
