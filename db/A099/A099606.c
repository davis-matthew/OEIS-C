/*
 * A099606  Row sums of triangle A099605, in which row n equals the inverse
 *          Binomial transform of column n of the triangle A034870 of
 *          even-indexed rows of Pascal's triangle.
 * https://oeis.org/A099606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pell(n+1)*2^[(n+1)/2] Recurrence: a(0) = 1, a(1) = 4,
 *            a(2) = 10, a(3) = 48, a(n) = 12*a(n-2) - 4*a(n-4) for n ≥ 4 (All
 *            arithmetic is performed in the type seq_t, which may be an
 *            unsigned integer or an arbitrary‑precision integer.)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if __cplusplus >= 201103L
    /* C++ implementation using std::vector for memoisation */
    #include <vector>
    static std::vector<seq_t> cache;
    static bool cache_initialized = false;

    static void ensure_cache(idx_t n) {
        if (!cache_initialized) {
            cache.reserve(4);
            cache.push_back((seq_t)1);  /* a(0) */
            cache.push_back((seq_t)4);  /* a(1) */
            cache.push_back((seq_t)10); /* a(2) */
            cache.push_back((seq_t)48); /* a(3) */
            cache_initialized = true;
        }
        if (n < (idx_t)cache.size()) return;
        idx_t oldsize = (idx_t)cache.size();
        cache.resize(n + 1);
        for (idx_t k = 4; k <= n; ++k) {
            cache[k] = (seq_t)12 * cache[k-2] - (seq_t)4 * cache[k-4];
        }
    }

#else
    /* C implementation using malloc/realloc for memoisation */
    #include <stdlib.h>
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    static void ensure_cache(idx_t n) {
        if (cache == NULL) {
            cache_size = 4;
            cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
            if (cache == NULL) abort();
            cache[0] = (seq_t)1;  /* a(0) */
            cache[1] = (seq_t)4;  /* a(1) */
            cache[2] = (seq_t)10; /* a(2) */
            cache[3] = (seq_t)48; /* a(3) */
        }
        if (n + 1 <= cache_size) return;
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) abort();
        cache = new_cache;
        for (idx_t k = cache_size; k <= n; ++k) {
            cache[k] = (seq_t)12 * cache[k-2] - (seq_t)4 * cache[k-4];
        }
        cache_size = n + 1;
    }

#endif

/* External function providing the n‑th term of A099606. */
extern seq_t a(idx_t n) {
    ensure_cache(n);
#if __cplusplus >= 201103L
    return cache[(size_t)n];
#else
    return cache[n];
#endif
}
