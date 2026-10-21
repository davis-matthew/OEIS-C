/*
 * A208289  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A208289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 10*a(n-2) - 2*a(n-3) - 7*a(n-4) + a(n-6)
 *            Initial values: a(1)=10, a(2)=100, a(3)=390, a(4)=2090,
 *            a(5)=9900, a(6)=49130
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* -------------------------------------------------------------------------- */
/* Cache handling – compatible with both C and C++                            */
/* -------------------------------------------------------------------------- */
#ifdef __cplusplus
/* C++: use std::vector which calls constructors automatically */
static std::vector<seq_t> cache;          /* indices 0 .. n ; 0 is unused */
static void ensure_cache(idx_t n)
{
    if (cache.size() <= (size_t)n)        /* need at least n+1 elements            */
        cache.resize(n + 1);              /* new elements are default‑constructed */
}
#else
/* C: use malloc/realloc; seq_t is an unsigned integer type      */
static seq_t *cache = NULL;
static size_t cache_cap = 0; /* allocated size (>= n+1) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {                 /* allocation failure – leave cache as is */
            return;
        }
        cache = tmp;
        /* zero‑initialize any newly added slots */
        for (size_t i = cache_cap; i < needed; ++i)
            cache[i] = 0;
        cache_cap = needed;
    }
}
#endif

/* -------------------------------------------------------------------------- */
/* Public function – returns a(n) for n >= 1                                 */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 – n==0 is not used but handled */

    ensure_cache(n);

#ifdef __cplusplus
    /* return cached value if it is already known */
    if (cache[(size_t)n] != 0)
        return cache[(size_t)n];

    /* compute from the smallest unknown index up to n */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (cache[i] != 0) continue;          /* already computed */
        if (i <= 6) {                         /* hard‑code first six values */
            switch ((int)i) {
                case 1:  cache[i] = 10;          break;
                case 2:  cache[i] = 100;         break;
                case 3:  cache[i] = 390;         break;
                case 4:  cache[i] = 2090;        break;
                case 5:  cache[i] = 9900;        break;
                case 6:  cache[i] = 49130;       break;
            }
        } else { /* recurrence for i > 6 */
            cache[i] = cache[i-1] * 3
                     + cache[i-2] * 10
                     - cache[i-3] * 2
                     - cache[i-4] * 7
                     + cache[i-6];
        }
    }
    return cache[(size_t)n];
#else
    if (cache[(size_t)n] != 0)
        return cache[(size_t)n];

    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (cache[i] != 0) continue;
        if (i <= 6) {
            switch ((int)i) {
                case 1:  cache[i] = 10;          break;
                case 2:  cache[i] = 100;         break;
                case 3:  cache[i] = 390;         break;
                case 4:  cache[i] = 2090;        break;
                case 5:  cache[i] = 9900;        break;
                case 6:  cache[i] = 49130;       break;
            }
        } else {
            seq_t tmp = cache[i-1] * 3 + cache[i-2] * 10 + cache[i-6];
            seq_t sub = cache[i-3] * 2 + cache[i-4] * 7;
            cache[i] = tmp - sub;                 /* signed subtraction; seq_t is unsigned */
        }
    }
    return cache[(size_t)n];
#endif
}
