/*
 * A167075  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*∑_{i=1}^{12} a(n-i) – 210*a(n-13) for n≥14 Initial
 *            values are obtained from the numerator of the generating
 *            function (see OEIS entry). The recurrence automatically gives
 *            a(0)=1, a(1)=22, …, a(13)=161828205250505871, etc
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;  /* allocated capacity (number of slots) */
    static size_t len   = 0;  /* first free index (i.e. entries are [0..len-1]) */

    /* one‑time cache allocation */
    if (cache == NULL) {
        cap = 16;                            /* enough for the first terms */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* ensure the cache is large enough for the requested index */
    size_t idx = (size_t)n;
    while (len <= idx) {
        seq_t val;

        if (len <= 12) {                     /* n = 0 … 12, use numerator */
            seq_t sum = 0;
            for (size_t j = 1; j <= len; ++j)
                sum += (seq_t)20 * cache[len - j];
            seq_t ncoeff = (len == 0 ? 1 : 2); /* N(t) coefficients */
            val = ncoeff + sum;
        } else if (len == 13) {              /* n = 13, special handling */
            seq_t sum12 = 0;
            for (size_t j = 1; j <= 12; ++j)
                sum12 += (seq_t)20 * cache[len - j];
            val = (seq_t)1 + sum12 - (seq_t)210 * cache[0];
        } else {                             /* n ≥ 14, homogeneous recurrence */
            seq_t sum12 = 0;
            for (size_t j = 1; j <= 12; ++j)
                sum12 += (seq_t)20 * cache[len - j];
            val = sum12 - (seq_t)210 * cache[len - 13];
        }

        /* grow the cache if necessary */
        if (len == cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cap   = newcap;
        }

        cache[len++] = val;                   /* store the computed term */
    }

    return cache[idx];
}
