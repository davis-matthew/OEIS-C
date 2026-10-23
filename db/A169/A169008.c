/*
 * A169008  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A169008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 ; For 1 ≤ n ≤ 22: a(n) = p[n] + 41 * Σ_{i=1}^{n}
 *            a(n-i), where p[0] = 1 and p[1…22] = 2. For n ≥ 23: a(n) = 41 *
 *            Σ_{i=1}^{22} a(n-i) – 861 * a(n-23). The sequence is generated
 *            by the rational generating function (t^23 + 2t^22 + … + 2t + 1)
 *            / (861 t^23 – 41 t^22 – … – 41 t + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed terms      */
static size_t cache_cap = 0;      /* allocated size of cache array*/
static size_t cache_len = 0;      /* number of terms currently stored */

/* Coefficients of numerator (p) and the recurrence coefficients (c). */
static const seq_t p_arr[24] = {
    (seq_t)1, /* p[0] */
    (seq_t)2, /* p[1] */
    (seq_t)2, /* p[2] */
    (seq_t)2, /* p[3] */
    (seq_t)2, /* p[4] */
    (seq_t)2, /* p[5] */
    (seq_t)2, /* p[6] */
    (seq_t)2, /* p[7] */
    (seq_t)2, /* p[8] */
    (seq_t)2, /* p[9] */
    (seq_t)2, /* p[10] */
    (seq_t)2, /* p[11] */
    (seq_t)2, /* p[12] */
    (seq_t)2, /* p[13] */
    (seq_t)2, /* p[14] */
    (seq_t)2, /* p[15] */
    (seq_t)2, /* p[16] */
    (seq_t)2, /* p[17] */
    (seq_t)2, /* p[18] */
    (seq_t)2, /* p[19] */
    (seq_t)2, /* p[20] */
    (seq_t)2, /* p[21] */
    (seq_t)2, /* p[22] */
    (seq_t)1  /* p[23] */
};

static const seq_t c_coeff[24] = {
    (seq_t)0,         /* unused, 1‑based indexing */
    (seq_t)41,        /* c[1]  – 41  */
    (seq_t)41,        /* c[2]  – 41  */
    (seq_t)41,        /* c[3]  – 41  */
    (seq_t)41,        /* c[4]  – 41  */
    (seq_t)41,        /* c[5]  – 41  */
    (seq_t)41,        /* c[6]  – 41  */
    (seq_t)41,        /* c[7]  – 41  */
    (seq_t)41,        /* c[8]  – 41  */
    (seq_t)41,        /* c[9]  – 41  */
    (seq_t)41,        /* c[10] – 41  */
    (seq_t)41,        /* c[11] – 41  */
    (seq_t)41,        /* c[12] – 41  */
    (seq_t)41,        /* c[13] – 41  */
    (seq_t)41,        /* c[14] – 41  */
    (seq_t)41,        /* c[15] – 41  */
    (seq_t)41,        /* c[16] – 41  */
    (seq_t)41,        /* c[17] – 41  */
    (seq_t)41,        /* c[18] – 41  */
    (seq_t)41,        /* c[19] – 41  */
    (seq_t)41,        /* c[20] – 41  */
    (seq_t)41,        /* c[21] – 41  */
    (seq_t)41,        /* c[22] – 41  */
    (seq_t)861       /* c[23] – 861 (negative in recurrence) */
};

seq_t a(idx_t n)
{
    /* Ensure cache can hold up to index n. */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 32;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i <= 22) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= i; ++j) {
                sum += (seq_t)41 * cache[i - j];
            }
            cache[i] = p_arr[i] + sum;
        } else {
            seq_t sum22 = 0;
            for (idx_t j = 1; j <= 22; ++j) {
                sum22 += (seq_t)41 * cache[i - j];
            }
            seq_t term23 = (seq_t)861 * cache[i - 23];
            cache[i] = sum22 - term23;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
