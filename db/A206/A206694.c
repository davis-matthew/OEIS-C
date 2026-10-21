/*
 * A206694  Number of n X 2 0..2 arrays avoiding the pattern z-2 z-1 z in any
 *          row or column.
 * https://oeis.org/A206694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 3*a(n-2) - 25*a(n-3) + 9*a(n-4) + 3*a(n-5) -
 *            a(n-6) for n >= 7 with initial values a(1)=9, a(2)=81, a(3)=676,
 *            a(4)=5625, a(5)=46656, a(6)=386884
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* maximum n that has already been computed and stored */
static idx_t max_cached = 0;

/* storage for the sequence values */
#ifdef __cplusplus
static std::vector<seq_t> cacheVec;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
#endif

/* Ensure the storage can hold at least n elements. */
static void ensure_capacity(idx_t n)
{
#ifdef __cplusplus
    if (cacheVec.size() < n)
        cacheVec.resize(n);
#else
    if (!cache || cache_cap < (size_t)n) {
        size_t new_cap = n;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);            /* out‑of‑memory: abort */
        cache = new_cache;
        cache_cap = new_cap;
    }
#endif
}

/* Return a pointer to the underlying array (vector data or malloced). */
static seq_t* cache_ptr(void)
{
#ifdef __cplusplus
    return cacheVec.data();
#else
    return cache;
#endif
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, not part of the definition */
    ensure_capacity(n);

    if (n > max_cached) {
        seq_t *arr = cache_ptr();
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            idx_t idx = i - 1;    /* zero‑based index */
            if (i == 1)          arr[idx] = 9;
            else if (i == 2)      arr[idx] = 81;
            else if (i == 3)      arr[idx] = 676;
            else if (i == 4)      arr[idx] = 5625;
            else if (i == 5)      arr[idx] = 46656;
            else if (i == 6)      arr[idx] = 386884;
            else {
                /* Positives and negatives are grouped to avoid unsigned
                   underflow, since the sequence is strictly increasing. */
                seq_t pos = 9 * arr[idx-1] + 9 * arr[idx-4] + 3 * arr[idx-5];
                seq_t neg = 3 * arr[idx-2] + 25 * arr[idx-3] + arr[idx-6];
                arr[idx] = pos - neg;
            }
        }
        max_cached = n;
    }
    return cache_ptr()[n-1];
}
