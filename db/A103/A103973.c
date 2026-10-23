/*
 * A103973  Expansion of (sqrt(1-8*x^2)+8*x^2+2*x-1)/(2*x*sqrt(1-8*x^2)).
 * https://oeis.org/A103973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} 2^(n‑k) * Catalan(k) * C(k+1, n‑k)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom, catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility functions used (provided by the build) */
#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp)           */
#include "../utils/catalan.h" /* seq_t catalan(idx_t k)          */
#include "../utils/binom.h"   /* seq_t binom(idx_t N, idx_t K)   */

seq_t a(idx_t n)
{
    idx_t maxk = n >> 1;      /* floor(n/2) */
    seq_t total = (seq_t)0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t p    = pow2(n - k);                 /* 2^(n‑k)          */
        seq_t c    = catalan(k);                  /* Catalan(k)       */
        seq_t b    = binom(k + 1, n - k);         /* C(k+1, n‑k)      */
        total += p * c * b;
    }
    return total;
}
