/*
 * A229457  Number of defective 3-colorings of an nX6 0..2 array connected
 *          horizontally and vertically with exactly one mistake and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(n) = 60*a(n-1) -1482*a( n-2)
 *            +20016*a(n-3) -167163*a(n-4) +924894*a(n-5) -3537110*a(n-6)
 *            +9593166*a(n-7) -18693789*a(n-8) +26218062*a(n-9)
 *            -26188161*a(n-10) +18139608*a(n-11) -8262712*a(n-12)
 *            +2220864*a(n-13) -266256*a(n-14), for n > 15. The first 16
 *            values are...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"

static const int COEFFS[14] = {
    60, -1482, 20016, -167163, 924894, -3537110,
    9593166, -18693789, 26218062, -26188161,
    18139608, -8262712, 2220864, -266256
};
static const int OFFSETS[14] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
};

static seq_t* cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static bool init_done = false;

static void init_cache(void) {
    const char *strs[17] = {
        NULL, /* 0 unused */
        "40",
        "1188",
        "27313",
        "573342",
        "11451686",
        "221410052",
        "4182294415",
        "77626332302",
        "1421182695065",
        "25734458996766",
        "461813205678124",
        "8225319344357866",
        "145570937702109584",
        "2562275384434847728",
        "44887086267708866341",
        "783101106222482644104"
    };
    /* Minimum capacity to hold first 16 terms plus index 0 */
    cache_cap = 17;
    cache = (seq_t*)malloc(sizeof(seq_t) * (cache_cap + 1));
    for (size_t i = 1; i <= 16; ++i) {
        cache[i] = seq_from_str(strs[i]);
    }
    cache_len = 16;
    init_done = true;
}

static void ensure_cache(idx_t n) {
    if (!init_done) init_cache();
    if ((idx_t)(cache_len) >= n) return;
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        cache = tmp;
        cache_cap = new_cap;
    }
    /* compute missing entries */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (int k = 0; k < 14; ++k) {
            idx_t idx = i - OFFSETS[k];
            seq_t term = cache[idx];
            if (COEFFS[k] > 0) {
                pos += term * (seq_t)COEFFS[k];
            } else {
                neg += term * (seq_t)(-COEFFS[k]);
            }
        }
        cache[i] = pos - neg; /* subtraction is safe: pos >= neg */
    }
    cache_len = n;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0; /* undefined for n < 1, return 0 */
    ensure_cache(n);
    return cache[n];
}
