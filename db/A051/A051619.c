/*
 * A051619  a(n) = (4*n+7)(!^4)/7(!^4), related to A034176(n+1) ((4*n+3)(!^4)
 *          quartic, or 4-factorials).
 * https://oeis.org/A051619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n+7) * a(n-1), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation – use std::vector to handle arbitrary‑precision types */
    static std::vector<seq_t> cache{(seq_t)1};   /* a(0) */
    if (n < cache.size())
        return cache[(size_t)n];
    /* compute missing terms up to n */
    for (size_t i = cache.size(); i <= n; ++i)
    {
        seq_t factor = (seq_t)4 * (seq_t)i + (seq_t)7;
        cache.push_back(cache.back() * factor);
    }
    return cache[(size_t)n];
#else
    /* C implementation – dynamic array via malloc/realloc */
    static seq_t *cache = NULL;
    static idx_t   computed = 0;   /* highest index computed so far */
    static size_t  cap      = 0;   /* allocated capacity */

    /* first time: allocate and set a(0) = 1 */
    if (!cache)
    {
        cap = 1;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();      /* allocation failed */
        cache[0] = (seq_t)1;
        computed = 0;
    }

    if (n <= computed)
        return cache[(size_t)n];

    /* ensure enough capacity */
    size_t needed = (size_t)(n + 1);
    if (needed > cap)
    {
        size_t new_cap = (cap ? cap * 2 : 16);
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute and store missing terms */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        seq_t factor = (seq_t)4 * (seq_t)i + (seq_t)7;
        cache[(size_t)i] = cache[(size_t)(i - 1)] * factor;
    }
    computed = n;
    return cache[(size_t)n];
#endif
}
