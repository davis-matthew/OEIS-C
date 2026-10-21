/*
 * A077446  Numbers k such that 2*k^2 + 14 is a square.
 * https://oeis.org/A077446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077446: Numbers k such that 2*k^2 + 14 is a square
   Recurrence: a(1)=1, a(2)=5, a(3)=11, a(4)=31,
                a(n) = 6*a(n-2) - a(n-4)   for n ≥ 5
   The function memoises already‑computed terms so that calls can
   arrive in any order. */

#ifdef __cplusplus
#include <vector>
#else
#include <stddef.h>
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* return 0 for non‑positive indices – the offset is 1 */
    if (n < 1) return (seq_t)0;

#ifdef __cplusplus
    /* C++ implementation using a dynamic std::vector */
    static std::vector<seq_t> cache;         /* cache[i] holds a(i+1) */
    /* extend cache until it contains term n */
    while (cache.size() < (size_t)n)
    {
        idx_t idx = (idx_t)(cache.size() + 1);   /* 1‑based index of new term */
        seq_t val;
        if (idx == 1)          val = (seq_t)1;
        else if (idx == 2)     val = (seq_t)5;
        else if (idx == 3)     val = (seq_t)11;
        else if (idx == 4)     val = (seq_t)31;
        else                   val = (seq_t)6 * cache[idx - 3] - cache[idx - 5];
        cache.push_back(val);
    }
    return cache[(size_t)(n - 1)];
#else
    /* C implementation using a malloc’ed array */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;       /* number of terms currently stored (a(1) at cache[0]) */

    /* allocate or grow the cache if needed */
    if (cache_len < (size_t)n)
    {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {                 /* allocation failure */
            cache = tmp;
            return (seq_t)0;
        }
        cache = tmp;
        cache_len = new_len;

        /* compute missing terms */
        for (size_t i = cache_len - (size_t)n; i < cache_len; ++i)
        {
            idx_t idx = (idx_t)(i + 1);           /* 1‑based index */
            seq_t val;
            if (idx == 1)          val = (seq_t)1;
            else if (idx == 2)     val = (seq_t)5;
            else if (idx == 3)     val = (seq_t)11;
            else if (idx == 4)     val = (seq_t)31;
            else                  val = (seq_t)6 * cache[idx - 3] - cache[idx - 5];
            cache[i] = val;
        }
    }
    return cache[(size_t)(n - 1)];
#endif
}
