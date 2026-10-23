/*
 * A194827  2-adic valuation of the number of n X n Alternating Sign Matrices
 *          (A005130(n)).
 * https://oeis.org/A194827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = a(n-1) + popcount(n-1) - popcount(3*n-2) + 1 (n≥2) with
 *            a(1) = 0. popcount(n) = A000120(n) (binary digit sum)
 * Range:     u8 n <= 895, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* portable popcount for 64‑bit values */
static inline int popcount_u64(unsigned long long x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    int c = 0;
    while (x) { x &= x-1; ++c; }
    return c;
#endif
}

#ifdef __cplusplus
/* ---- C++ implementation using std::vector (constructs elements) ---- */
# include <vector>
static std::vector<seq_t> cache_vec;
static idx_t cache_last = 0;   /* largest index already computed   */

static void ensure_cache(idx_t n) {
    /* Ensure vector has space for indices up to n.
       The vector elements are default‑constructed when resized. */
    if (n < cache_vec.size()) return;
    cache_vec.resize(n + 1);   /* indices 0..n inclusive; 0 unused     */
}
#else
/* ---- C implementation using malloc/realloc (no constructors needed) ---- */
static seq_t *cache = NULL;    /* index 0 unused, cache[1] = a(1)      */
static idx_t cache_cap = 0;    /* allocated size (without index 0)    */
static idx_t cache_last = 0;   /* largest index already computed      */

static void ensure_cache(idx_t n) {
    if (n <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < n) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, (size_t)(new_cap + 1) * sizeof(seq_t));
    cache_cap = new_cap;
}
#endif

/* Main function required by the problem. */
seq_t a(idx_t n) {
    if (n < 1) return 0;               /* undefined, default to 0          */

#ifdef __cplusplus
    ensure_cache(n);
    if (cache_last >= n) return cache_vec[(size_t)n];
    /* compute from cache_last+1 .. n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        int pc_i_minus_1  = popcount_u64((unsigned long long)(i - 1));
        int pc_3i_minus_2 = popcount_u64((unsigned long long)(3 * i - 2));
        cache_vec[(size_t)i] = cache_vec[(size_t)(i-1)]
                              + (seq_t)pc_i_minus_1
                              - (seq_t)pc_3i_minus_2
                              + 1;
    }
    cache_last = n;
    return cache_vec[(size_t)n];
#else
    ensure_cache(n);
    if (cache_last >= n) return cache[(size_t)n];

    /* initialise a(1) if not yet done */
    if (cache_last == 0) {
        cache[(size_t)1] = 0;
        cache_last = 1;
    }

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        int pc_i_minus_1  = popcount_u64((unsigned long long)(i - 1));
        int pc_3i_minus_2 = popcount_u64((unsigned long long)(3 * i - 2));
        cache[(size_t)i] = cache[(size_t)(i-1)]
                           + (seq_t)pc_i_minus_1
                           - (seq_t)pc_3i_minus_2
                           + 1;
    }
    cache_last = n;
    return cache[(size_t)n];
#endif
}
