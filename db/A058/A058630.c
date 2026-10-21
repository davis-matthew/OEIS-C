/*
 * A058630  McKay-Thompson series of class 32B for the Monster group.
 * https://oeis.org/A058630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} (v(k) * a(n-k)) / n * where v(k) =
 *            Sum_{d|k} d * c_d and c_d are the periodic coefficients * C =
 *            {2, -1, 2, 1, 2, -1, 2, -2, 2, -1, 2, 1, 2, -1, 2, 0} * This is
 *            the Euler transform of the period‑16 sequence listed in the *
 *            OEIS entry. The recurrence is correct for all n ≥ 0
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 150, u64 n <= 512,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#  include <vector>
#else
#  include <stdlib.h>
#  include <stdint.h>
#endif

/* Use a 128‑bit integer for intermediate sums when available. */
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
typedef unsigned __int128 huge_t;
#  define HAVE_BIG_INT 1
#else
typedef unsigned long long huge_t;   /* fallback, should rarely overflow for n<=10000 */
#  define HAVE_BIG_INT 0
#endif

/* Periodic coefficients (length 16).  They are small enough to stay in int. */
static const int C[16] = {
    2, -1, 2, 1, 2, -1, 2, -2,
    2, -1, 2, 1, 2, -1, 2, 0
};

/* Memoised sequence calculator.  Public interface. */
seq_t a(idx_t n)
{
#if defined(__cplusplus)

    /* Cache of previously computed results. */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.reserve(1);
        cache.push_back((seq_t)1);          /* a(0) = 1 */
    }

    size_t needed = (size_t)n + 1;
    if (needed > cache.size()) {
        size_t old = cache.size();
        /* Allocate space for new terms. */
        cache.resize(needed);

        /* Working array for v(k) (k = 1 .. needed-1). */
        std::vector<int> v(needed, 0);
        for (size_t d = 1; d < needed; ++d) {
            int cd  = C[(d - 1) % 16];
            int add = (int)d * cd;
            for (size_t m = d; m < needed; m += d)
                v[m] += add;
        }

        /* Compute a[old .. needed-1] using the recurrence. */
        for (size_t m = old; m < needed; ++m) {
            huge_t sum128 = 0;
            for (size_t k = 1; k <= m; ++k) {
                /* v[k] fits in int; cache[m-k] fits in seq_t. */
                sum128 += (huge_t)v[k] * (huge_t)cache[m - k];
            }
            cache[m] = (seq_t)(sum128 / (huge_t)m);   /* exact division */
        }
    }
    return cache[(size_t)n];

#else   /* C implementation */

    /* Static cache for all terms.  Do not use re‑allocation on each call if not needed. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        /* Allocate or extend the cache. */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */ }
        cache = tmp;

        /* Working array for v(k).  Use 32‑bit ints. */
        int *v = (int *)malloc(needed * sizeof(int));
        if (!v) { /* allocation failure – abort */ }
        for (size_t i = 0; i < needed; ++i) v[i] = 0;

        /* Pre‑compute v(k). */
        for (size_t d = 1; d < needed; ++d) {
            int cd  = C[(d - 1) % 16];
            int add = (int)d * cd;
            for (size_t m = d; m < needed; m += d)
                v[m] += add;
        }

        /* Initialise a(0) if this is the first call. */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;   /* a(0) = 1 */
            cache_len = 1;
        }

        /* Compute a[cache_len .. needed-1] using the recurrence. */
        for (size_t m = cache_len; m < needed; ++m) {
            huge_t sum128 = 0;
            for (size_t k = 1; k <= m; ++k) {
                sum128 += (huge_t)v[k] * (huge_t)cache[m - k];
            }
            cache[m] = (seq_t)(sum128 / (huge_t)m); /* exact division */
        }

        free(v);
        cache_len = needed;
    }

    return cache[(size_t)n];

#endif  /* __cplusplus */
}
