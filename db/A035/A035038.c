/*
 * A035038  a(n) = 2^n - C(n,0) - C(n,1) - ... - C(n,5).
 * https://oeis.org/A035038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n – Σ_{k=0..5} binomial(n,k) Equivalent to a(n) =
 *            Σ_{k=6..n} binomial(n,k)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;

    /* Compute Σ_{k=0..5} C(n,k) using multiplicative update */
    seq_t sum  = 1;               /* C(n,0) */
    seq_t term = 1;               /* current binomial term */

    for (idx_t k = 1; k <= 5; ++k) {
        term = term * (seq_t)(n - k + 1) / (seq_t)k;  /* C(n,k) from C(n,k-1) */
        sum += term;
    }

    seq_t pow2n = pow2(n);        /* 2^n */
    return pow2n - sum;           /* a(n) */
}
