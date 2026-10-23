/*
 * A170484  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A170484.
   The sequence satisfies the linear recurrence

        a(0)             = 1,
        a(n) = 2 + 41 * (a(0)+...+a(n-1))   for 1 ≤ n < 45,

        a(n) = 41 * (a(n-1)+...+a(n-44)) - 861 * a(n-45)  for n ≥ 45.

   All computations are performed in the opaque type `seq_t`, and the
   implementation uses a static cache so that subsequent calls are
   answered in O(1) amortised time.
*/

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(64);          /* initial capacity */
        cache.push_back(1);         /* a(0) */
    }

    if (n >= cache.size()) {
        size_t old = cache.size();
        cache.resize(n + 1);        /* grow to required index */

        for (size_t i = old; i <= static_cast<size_t>(n); ++i) {
            seq_t val;
            if (i == 0) {
                val = 1; /* a(0) already set */
            } else if (i < 45) {
                seq_t sum = 0;
                for (size_t j = 0; j < i; ++j) sum += cache[j];
                val = 2 + (seq_t)41 * sum;
            } else {
                seq_t sum = 0;
                for (size_t j = 1; j <= 44; ++j) sum += cache[i - j];
                val = (seq_t)41 * sum - (seq_t)861 * cache[i - 45];
            }
            cache[i] = val;
        }
    }

    return cache[static_cast<size_t>(n)];
#else
    /* C implementation using malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* first not-yet-stored index */
    static size_t cache_cap = 0;

    /* Grow cache when needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    while (cache_len <= n) {
        seq_t val;
        if (cache_len == 0) {
            val = 1; /* a(0) */
        } else if (cache_len < 45) {
            seq_t sum = 0;
            for (idx_t j = 0; j < cache_len; ++j) sum += cache[j];
            val = 2 + (seq_t)41 * sum;
        } else {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 44; ++j) sum += cache[cache_len - j];
            val = (seq_t)41 * sum - (seq_t)861 * cache[cache_len - 45];
        }
        cache[cache_len++] = val;
    }

    return cache[(size_t)n];
#endif
}
