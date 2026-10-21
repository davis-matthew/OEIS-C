/*
 * A054877  Closed walks of length n along the edges of a pentagon based at a
 *          vertex.
 * https://oeis.org/A054877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 5*(a(n‑2) – a(n‑4)) + 2*a(n‑5) Initial values: a(0)=1,
 *            a(1)=0, a(2)=2, a(3)=0, a(4)=6, a(5)=2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif


#if defined(__cplusplus)
/* For C++: use 128‑bit arithmetic only for built‑in integral seq_t’s */
static inline seq_t mul_small_cpp(seq_t a, unsigned int m)
{
    if constexpr (std::is_integral<seq_t>::value) {
        return (seq_t)((unsigned __int128)a * m);
    } else {
        return a * m;           /* assume arbitrary‑precision type supports * */
    }
}
#else
/* For plain C: seq_t is an unsigned integer type; use 128‑bit temp */
static inline seq_t mul_small_c(seq_t a, unsigned int m)
{
    return (seq_t)((unsigned __int128)a * m);
}
#endif

#if defined(__cplusplus)
# define MUL_SMALL(a,m) mul_small_cpp(a,m)
#else
# define MUL_SMALL(a,m) mul_small_c(a,m)
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise the first 6 terms if necessary */
    if (cache_size <= (idx_t)6) {
        if (cache) free(cache);
        cache_size = 6;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)6;
        cache[5] = (seq_t)2;
    }

    if (n < cache_size)                    /* already computed */
        return cache[(size_t)n];

    /* extend cache up to n */
    idx_t old_size = cache_size;
    cache_size = n + 1;
    cache = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));

    for (idx_t i = old_size; i <= n; ++i) {
        if (i < 6) continue;               /* base values already set */

        seq_t diff = cache[i-2] - cache[i-4];
        seq_t val  = MUL_SMALL(diff,5) + MUL_SMALL(cache[i-5],2);
        cache[i]   = val;
    }
    return cache[(size_t)n];
}
