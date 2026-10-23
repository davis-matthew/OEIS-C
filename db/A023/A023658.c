/*
 * A023658  Convolution of odd numbers and A000201.
 * https://oeis.org/A023658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{n} (2k-1) * ( ceil( (n-k+1)·φ ) - 1 ) where φ =
 *            (1 + √5) / 2 (the golden ratio)
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* The types seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* golden ratio as long double for sufficient precision   */
    static const long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;

    /*  result = (2n+1) * Σ P(t)  –  2 * Σ t·P(t),   where
       P(t) = ceil( t·φ ) – 1 .                      */
    seq_t coeff = (seq_t)(2 * n + 1);
    seq_t sumP     = 0;      /* Σ P(t)                 */
    seq_t sumTP    = 0;      /* Σ t·P(t)               */
    for (idx_t t = 1; t <= n; ++t)
    {
        /* P(t) = ceil(t·φ) – 1  (t is small – no overflow). */
        long double val = t * phi;
        /* subtract a tiny epsilon to guard against 1‑ulps
           floating‑point rounding that would otherwise give the
           wrong ceil when val is within 1e‑12 of an integer.   */
        idx_t ceil_val = (idx_t)ceill(val - 1e-12L);
        seq_t P = (seq_t)ceil_val - 1;

        sumP += P;
        sumTP += (seq_t)t * P;
    }

    return coeff * sumP - (seq_t)2 * sumTP;
}
