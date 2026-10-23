/*
 * A162482  Expansion of (1/(1-x)^3)*M(x/(1-x)^3), M(x) the g.f. of Motzkin
 *          numbers A001006.
 * https://oeis.org/A162482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0}^{n} C(n+2k+2, n−k) · A001006(k) Recurrence used
 *            (for n ≥ 6): (n+2)*a(n) = (8n+4)*a(n−1) –18(n−1)*a(n−2) +
 *            (26n−65)*a(n−3) –17(n−4)*a(n−4) + (6n−33)*a(n−5) –(n−7)*a(n−6)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first six terms, a(0) … a(5) */
    static const seq_t base[6] = {1, 4, 14, 53, 218, 945};

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 6)
        return base[n];

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;    /* out of memory */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = base[i];
    }

    /* extend the cached values up to n */
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;    /* out of memory */
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            /* positive‑coefficients terms 1,3,5 */
            seq_t term1 = ((seq_t)(8 * i + 4)) * cache[i - 1];
            seq_t term3 = ((seq_t)(26 * i - 65)) * cache[i - 3];
            seq_t term5 = ((seq_t)(6 * i - 33)) * cache[i - 5];

            seq_t S_pos = term1 + term3 + term5;

            /* negative‑coefficients terms 2,4 and term6 depending on i */
            seq_t term2 = ((seq_t)(18 * (i - 1))) * cache[i - 2];
            seq_t term4 = ((seq_t)(17 * (i - 4))) * cache[i - 4];
            seq_t S_neg = term2 + term4;

            seq_t term6_coeff;
            if (i < 7)            /* (i-7) is negative → contribution is positive */
                term6_coeff = (seq_t)(7 - i);
            else
                term6_coeff = (seq_t)(i - 7);

            if (i < 7)
                S_pos += term6_coeff * cache[i - 6];
            else
                S_neg += term6_coeff * cache[i - 6];

            /* numerator and division */
            seq_t numer = S_pos - S_neg;            /* exact division guaranteed */
            seq_t denom = (seq_t)(i + 2);
            cache[i] = numer / denom;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
