/*
 * A033849  Numbers whose prime factors are 3 and 5.
 * https://oeis.org/A033849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15 * A003593(n) where A003593(n) is the nth number of the
 *            form 3^i * 5^j (i,j ≥ 0)
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 123, u64 n <= 523,
 *            u128 n <= 2158, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but just in case */

#ifdef __cplusplus
    /*  C++17 mode: use a static std::vector for storage.  */
    static std::vector<seq_t> cache;
    static idx_t i3 = 0, i5 = 0;

    if (cache.empty())
    {
        cache.push_back((seq_t)1);   /* 3^0 * 5^0 = 1 */
    }

    while (cache.size() < n)
    {
        seq_t next3 = cache[i3] * (seq_t)3;
        seq_t next5 = cache[i5] * (seq_t)5;
        seq_t next  = next3 < next5 ? next3 : next5;
        cache.push_back(next);

        if (next == next3) ++i3;
        if (next == next5) ++i5;
    }

    return cache[n - 1] * (seq_t)15;

#else
    /*  C11 mode: use malloc/realloc for storage.  */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  i3 = 0, i5 = 0;

    if (cache_len == 0)
    {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;      /* allocation failure */
        cache[0] = (seq_t)1;   /* 3^0 * 5^0 = 1 */
        cache_len = 1;
    }

    /* Grow cache if necessary */
    while (cache_len < n)
    {
        /* Resize if needed */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;   /* allocation failure */
            cache    = tmp;
            cache_cap = new_cap;
        }

        seq_t next3 = cache[i3] * (seq_t)3;
        seq_t next5 = cache[i5] * (seq_t)5;
        seq_t next  = next3 < next5 ? next3 : next5;
        cache[cache_len++] = next;

        if (next == next3) ++i3;
        if (next == next5) ++i5;
    }

    return cache[n - 1] * (seq_t)15;
#endif
}
