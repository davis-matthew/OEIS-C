/*
 * A221084  Number of nX5 arrays of occupancy after each element moves to some
 *          horizontal or antidiagonal neighbor.
 * https://oeis.org/A221084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A221084:   a(n) = 192*a(n-1) - 9248*a(n-2) + 195456*a(n-3) - 2093824*a(n-4) + 11781120*a(n-5)
 *                  - 34549760*a(n-6) + 52838400*a(n-7) - 40501248*a(n-8) + 13369344*a(n-9) - 1048576*a(n-10)
 *  Offset: 1   (a(1) = 8)          */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* external types supplied by the build */
typedef idx_t idx_t;      /* 64‑bit unsigned index */
typedef seq_t seq_t;      /* unsigned integer of any width or an arbitrary‑precision integer class */

/* static cache to store computed terms */
static seq_t *cache = NULL;    /* array of seq_t, 1‑based (cache[0] unused) */
static idx_t cache_size = 0;   /* current allocated size (>=1) */

/* initial terms as strings to avoid 64‑bit truncation */
static const char *init_str[10] = {
    "8",
    "1155",
    "154440",
    "20489472",
    "2714713664",
    "359601021696",
    "47632473997312",
    "6309330479063040",
    "835724490894491648",
    "110698808144126017536"
};

/* helper: grow the cache to at least new_size elements (new_size >= 1) */
static void ensure_cache_size(idx_t new_size) {
    if (cache_size >= new_size) return;
    seq_t *new_cache;
    new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (!new_cache) exit(1);        /* allocation failed */
    /* zero-initialize the new slots for safety, though not strictly needed */
    for (idx_t i = cache_size; i < new_size; ++i) new_cache[i] = (seq_t)0;
    cache = new_cache;
    cache_size = new_size;
}

/* helper: initialize the first 10 terms (1‑based) */
static void init_first_ten(void) {
    ensure_cache_size(11);          /* indices 0..10 inclusive */
    for (idx_t i = 1; i <= 10; ++i) {
        cache[i] = seq_from_str(init_str[i - 1]);
    }
}

/* public function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;      /* undefined for n<1; return 0 */
    if (cache_size < 11) init_first_ten();   /* ensure first 10 terms are ready */

    if (n <= 10) return cache[n];

    ensure_cache_size(n + 1);        /* need indices up to n */

    /* compute terms from 11 up to n using the recurrence */
    for (idx_t i = 11; i <= n; ++i) {
        seq_t val;
        val = cache[i - 1] * (seq_t)192;
        val -= cache[i - 2] * (seq_t)9248;
        val += cache[i - 3] * (seq_t)195456;
        val -= cache[i - 4] * (seq_t)2093824;
        val += cache[i - 5] * (seq_t)11781120;
        val -= cache[i - 6] * (seq_t)34549760;
        val += cache[i - 7] * (seq_t)52838400;
        val -= cache[i - 8] * (seq_t)40501248;
        val += cache[i - 9] * (seq_t)13369344;
        val -= cache[i -10] * (seq_t)1048576;
        cache[i] = val;
    }
    return cache[n];
}
