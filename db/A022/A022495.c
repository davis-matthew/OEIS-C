/*
 * A022495  Conjectured number of irreducible multiple zeta values of depth 7
 *          and weight 2n+19.
 * https://oeis.org/A022495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of xⁿ in
 *            (1+4x+8x²+8x³+6x⁴+4x⁵+5x⁶+6x⁷+3x⁸-2x⁹-3x¹⁰-x¹¹+x¹²+x¹³) /
 *            ((1-x²)³·(1-x³)³·(1-x⁷))
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Denominator coefficients:  D(x)=∑_{k=0}^{22} d_k x^k  (d_0=1) */
static const int d_coeff[23] = {
     1,  0, -3, -3,  3,  9,  2, -10, -9,  5, 12,  0, -12, -5,  9, 10,
    -2, -9, -3,  3,  3,  0, -1
};

/* Numerator coefficients:  P(x)=∑_{k=0}^{13} p_k x^k  (pₙ=0 for n>13) */
static const int p_coeff[14] = {
      1,  4,  8,  8,  6,  4,  5,  6,  3, -2, -3, -1,  1,  1
};

static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size) return;

    size_t old_size = cache_size;
    cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    cache_size = needed;
    for (idx_t i = (idx_t)old_size; i < needed; ++i) {
        seq_t val = 0;
        if (i <= 13) {
            int pn = p_coeff[(int)i];
            if (pn > 0)        val += (seq_t)pn;
            else if (pn < 0)   val -= (seq_t)(-pn);
        }
        for (int k = 1; k <= 22; ++k) {
            if (i < (idx_t)k) continue;
            int dk = d_coeff[k];
            if (dk > 0)        val -= (seq_t)dk * cache[i - k];
            else if (dk < 0)   val += (seq_t)(-dk) * cache[i - k];
        }
        cache[i] = val;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
