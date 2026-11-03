/*
 * A103973  Expansion of (sqrt(1-8*x^2)+8*x^2+2*x-1)/(2*x*sqrt(1-8*x^2)).
 * https://oeis.org/A103973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} 2^(n‑k) * Catalan(k) * C(k+1, n‑k)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility functions used (provided by the build) */
#include <stdint.h>
#include <sys/types.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Catalan number C(k) = binom(2k,k)/(k+1) computed multiplicatively */
static seq_t catalan(idx_t k)
{
    seq_t res = (seq_t)1;
    /* compute binom(2k,k) */
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(k + i) / (seq_t)i;
    }
    /* divide by (k+1) to get Catalan number */
    res /= (seq_t)(k + 1);
    return res;
}

seq_t a(idx_t n)
{
    idx_t maxk = n >> 1;      /* floor(n/2) */
    seq_t total = (seq_t)0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t p    = pow2(n - k);                 /* 2^(n‑k)          */
        seq_t c    = catalan(k);                  /* Catalan(k)       */
        seq_t b    = binomial(k + 1, n - k);         /* C(k+1, n‑k)      */
        total += p * c * b;
    }
    return total;
}
