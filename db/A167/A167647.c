/*
 * A167647  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=50, a(2)=2450, a(3)=120050, a(4)=5882450,
 *            a(5)=288240050, a(6)=14123762450, a(7)=692064360050,
 *            a(8)=33911153642450, a(9)=1661646528480050,
 *            a(10)=81420679895522450, a(11)=3989613314880600050,
 *            a(12)=195491052429149402450, a(13)=9579061569028320720050,
 *            a(14)=469374016882387715281225; Fo...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* 15 initial terms as strings (fits any seq_t). */
    static const char *init_str[15] = {
        "1",
        "50",
        "2450",
        "120050",
        "5882450",
        "288240050",
        "14123762450",
        "692064360050",
        "33911153642450",
        "1661646528480050",
        "81420679895522450",
        "3989613314880600050",
        "195491052429149402450",
        "9579061569028320720050",
        "469374016882387715281225"
    };

    static seq_t *cache = NULL;      /* array of terms */
    static idx_t size  = 0;          /* number of terms stored (0 … size‑1) */
    static idx_t cap   = 0;          /* allocated capacity */

    /* Allocate and initialize cache on first call. */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */

        for (idx_t i = 0; i < 15; ++i)
            cache[i] = seq_from_str(init_str[i]);

        size = 15;   /* terms 0 … 14 are cached */
    }

    /* Return cached value if already computed. */
    if (n < size)
        return cache[n];

    /* Grow cache if necessary. */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;              /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n (inclusive). */
    for (idx_t k = size; k <= n; ++k) {
        /* Recurrence applies for k ≥ 15. */
        if (k >= 15) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 13; ++j)
                sum += cache[k - j];
            cache[k] = ((seq_t)48) * sum - ((seq_t)1176) * cache[k - 14];
        } else {
            /* k < 15: already initialized, safety check. */
            cache[k] = seq_from_str(init_str[k]);
        }
    }
    size = n + 1;   /* we now have terms up to n */

    return cache[n];
}
