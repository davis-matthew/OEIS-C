/*
 * A218807  Hilltop maps: number of nX5 binary arrays indicating the locations
 *          of corresponding elements not exceeded by any horizontal or
 *          antidiagonal neighbor in a random 0..3 nX5 array.
 * https://oeis.org/A218807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*a(n-1) + 88*a(n-2) + 236*a(n-3) + 503*a(n-4) +
 *            361*a(n-5) + 156*a(n-6) - 348*a(n-7) + 73*a(n-8) - 37*a(n-9) -
 *            36*a(n-10) - 15*a(n-12) + 15*a(n-13) for n > 14, with initial
 *            values a(1) = 29, a(2) = 993, a(3) = 31741, a(4) = 1015637, a(5)
 *            = 32496361, a(6) = 1039771513, a(7) = 33269071149,...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Parse a decimal string into seq_t (supports arbitrary-precision types) */
static seq_t parse_decimal(const char *s) {
    seq_t v = 0;
    while (*s) {
        v = v * 10 + (seq_t)(*s - '0');
        ++s;
    }
    return v;
}

/* Static storage for memoised values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* Integer constants for the initial 15 terms */
static const char *const base_str[15] = {
    "29", "993", "31741", "1015637", "32496361",
    "1039771513", "33269071149", "1064494567893",
    "34060123730017", "1089805494659041", "34869985369949693",
    "1115718250329709413", "35699103424100012857",
    "1142247144301883171769", "36547935760902073717133"
};

/* Ensure the cache can hold at least n terms (1‑based) */
static void ensure_cache_size(idx_t n) {
    if (n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < (size_t)n)
        new_cap = (size_t)n;
    /* Reallocation with proper cast for both C and C++ */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 15 known terms */
static void init_cache(void) {
    if (cache_len != 0)
        return;
    /* Allocate enough space for 15 terms */
    if (cache_cap < 15)
        ensure_cache_size(15);
    for (size_t i = 0; i < 15; ++i)
        cache[i] = parse_decimal(base_str[i]);
    cache_len = 15;
}

/* The function required by the system */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* not defined for n = 0, but avoid UB */
    init_cache();
    if (n <= cache_len)
        return cache[n - 1];
    /* Grow the cache to accommodate n */
    ensure_cache_size(n);
    /* Compute terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Compute positive and negative contributions separately to avoid
           unsigned underflow on intermediate subtraction. */
        seq_t pos = 29 * cache[i - 2] + 88 * cache[i - 3] + 236 * cache[i - 4] +
                     503 * cache[i - 5] + 361 * cache[i - 6] + 156 * cache[i - 7] +
                     73  * cache[i - 9] + 15  * cache[i - 14];
        seq_t neg = 348 * cache[i - 8] + 37 * cache[i - 10] +
                     36 * cache[i - 11] + 15 * cache[i - 13];
        cache[i - 1] = pos - neg;
    }
    cache_len = n;
    return cache[n - 1];
}
