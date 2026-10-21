/*
 * A205253  Number of (n+1)X7 0..1 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock the same.
 * https://oeis.org/A205253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56*a(n-1) -1164*a(n-2) +12439*a(n-3) -78536*a(n-4)
 *            +314610*a(n-5) -829844*a(n-6) +1464316*a(n-7) * -1728640*a(n-8)
 *            +1345964*a(n-9) -671600*a(n-10) +205552*a(n-11) -36416*a(n-12)
 *            +3392*a(n-13) -128*a(n-14) * with a(1)=2920, a(2)=71000, ...,
 *            a(14)=4499358353656818662432
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

static const int coeff[14] = {
    56, -1164, 12439, -78536, 314610, -829844, 1464316,
    -1728640, 1345964, -671600, 205552, -36416, 3392, -128
};

static const char *base_str[14] = {
    "2920",
    "71000",
    "1764364",
    "44183032",
    "1109832184",
    "27913330476",
    "702420750928",
    "17679917387408",
    "445045123432100",
    "11203277447970640",
    "282028756620227128",
    "7099777698513424924",
    "178729975670304808352",
    "4499358353656818662432"
};

seq_t a(idx_t n) {
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored so far */
    static size_t cache_cap = 0;   /* allocated capacity */
    static bool initialized = false;

    /* One‑time initialisation: parse the first 14 terms. */
    if (!initialized) {
        cache_cap = 64;                          /* initial allocation size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (int i = 0; i < 14; ++i) {
            cache[i] = seq_from_str(base_str[i]);
        }
        cache_len = 14;
        initialized = true;
    }

    /* Grow the cache if the requested index is beyond what we have. */
    if ((idx_t)cache_len < n) {
        while ((idx_t)cache_len < n) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            size_t k = cache_len;           /* current term index (0‑based) */
            seq_t term = 0;
            for (int j = 0; j < 14; ++j) {
                int c = coeff[j];
                seq_t v = cache[k - 1 - j];
                if (c > 0) {
                    term += v * (seq_t)c;
                } else {
                    term -= v * (seq_t)(-c);
                }
            }
            cache[cache_len++] = term;
        }
    }

    return cache[n - 1];
}
