/*
 * A206148  Number of (n+1) X 7 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three counterclockwise and three clockwise edge
 *          increases.
 * https://oeis.org/A206148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A206148 implementation.
   Recurrence: a(n) = 55 a(n-1) -1109 a(n-2) +11330 a(n-3) -67206 a(n-4)
                +247404 a(n-5) -582440 a(n-6) +881876 a(n-7) -846764 a(n-8)
                +499200 a(n-9) -172400 a(n-10) +33152 a(n-11) -3264 a(n-12)
                +128 a(n-13)
   with the initial 13 terms given by the OEIS entry. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"   /* for seq_from_str */

static const char *const init_strs[14] = {
    /* 1‑based indices; init_strs[0] unused. */
    NULL,
    "4374",
    "106494",
    "2646540",
    "66274542",
    "1664748270",
    "41869995708",
    "1053631126386",
    "26519876081106",
    "667567685148144",
    "16804916171955954",
    "423043134930340686",
    "10649666547770137380",
    "268094963505457212522"
};

static seq_t *cache = NULL;      /* 1‑indexed cache of computed terms */
static idx_t cache_cap = 0;      /* current capacity (max index stored) */

/* Ensure the cache can hold values up to 'n' (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (n > 13 ? n : 13) + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(1);
        /* initialise with zeros */
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        /* copy known initial terms */
        for (idx_t i = 1; i <= 13 && i < cache_cap; ++i)
            cache[i] = seq_from_str(init_strs[i]);
    } else if (n >= cache_cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(1);
        cache = tmp;
        for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }
}

/* Compute the sequence term a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1 */

    ensure_cache(n);

    if (n <= 13) return cache[n];

    /* If terms up to n are already computed, just return. */
    if (cache[n] != 0) return cache[n];

    /* Offsets and coefficients for the recurrence. */
    static const int pos_offs[7]  = {1, 3, 5, 7, 9, 11, 13};
    static const seq_t pos_coeffs[7] = {55ULL, 11330ULL, 247404ULL,
                                        881876ULL, 499200ULL, 33152ULL,
                                        128ULL};
    static const int neg_offs[6]  = {2, 4, 6, 8, 10, 12};
    static const seq_t neg_coeffs[6] = {1109ULL, 67206ULL, 582440ULL,
                                        846764ULL, 172400ULL, 3264ULL};

    /* Compute sequentially from the smallest missing index up to n. */
    for (idx_t i = 14; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 7; ++k) {
            pos += cache[i - pos_offs[k]] * pos_coeffs[k];
        }
        for (int k = 0; k < 6; ++k) {
            neg += cache[i - neg_offs[k]] * neg_coeffs[k];
        }
        cache[i] = pos - neg;          /* true arithmetic; no underflow */
    }
    return cache[n];
}
