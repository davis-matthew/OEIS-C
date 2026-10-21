/*
 * A213766  Principal diagonal of the convolution array A213765.
 * https://oeis.org/A213766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) - 3*a(n-3) + 6*a(n-4) + a(n-5) -
 *            a(n-6) initial values: a(1)=1, a(2)=5, a(3)=24, a(4)=83,
 *            a(5)=263, a(6)=776
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* static cache of computed terms */
#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
#endif

/* ensure cache contains at least n terms (n >= 1) */
static void ensure_cache(idx_t n)
{
#ifdef __cplusplus
    if (n <= cache.size()) return;
    while (cache.size() < n) {
        size_t i = cache.size();          /* current index (0‑based) */
        seq_t val;
        if (i == 0)          val = (seq_t)1;
        else if (i == 1)     val = (seq_t)5;
        else if (i == 2)     val = (seq_t)24;
        else if (i == 3)     val = (seq_t)83;
        else if (i == 4)     val = (seq_t)263;
        else if (i == 5)     val = (seq_t)776;
        else {
            val = 5   *cache[i-1]
              - 6   *cache[i-2]
              - 3   *cache[i-3]
              + 6   *cache[i-4]
              +     cache[i-5]
              -     cache[i-6];
        }
        cache.push_back(val);
    }
#else
    if (n <= cache_size) return;
    while (cache_size < n) {
        seq_t val;
        if (cache_size == 0)          val = (seq_t)1;
        else if (cache_size == 1)     val = (seq_t)5;
        else if (cache_size == 2)     val = (seq_t)24;
        else if (cache_size == 3)     val = (seq_t)83;
        else if (cache_size == 4)     val = (seq_t)263;
        else if (cache_size == 5)     val = (seq_t)776;
        else {
            val = 5   *cache[cache_size-1]
              - 6   *cache[cache_size-2]
              - 3   *cache[cache_size-3]
              + 6   *cache[cache_size-4]
              +     cache[cache_size-5]
              -     cache[cache_size-6];
        }

        /* grow cache if necessary */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();      /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = val;
    }
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset 1: a(1) is first term */
    ensure_cache(n);
#ifdef __cplusplus
    return cache[n-1];
#else
    return cache[n-1];
#endif
}
