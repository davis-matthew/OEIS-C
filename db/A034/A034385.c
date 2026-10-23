/*
 * A034385  Expansion of (1-16*x)^(-1/4), related to quartic factorial
 *          numbers.
 * https://oeis.org/A034385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A034385: Expansion of (1‑16·x)^(-1/4)
   a(0) = 1
   For n ≥ 1:  a(n) = a(n‑1) * (16*n – 12) / n
   (Derived from the recurrence n·a(n)+4·(–4n+3)·a(n‑1)=0) */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* helper: multiply a by num then divide by den using a wider type
   to avoid overflow on integral seq_t.  For big‑int types ordinary
   operators are used. */
#ifdef __cplusplus
/* Integral variant – use 128‑bit intermediate */
template <typename T,
          typename std::enable_if_t<std::is_integral<T>::value, int> = 0>
static inline T mul_div(T a, T num, T den)
{
    return (T)((unsigned __int128)a * (unsigned __int128)num / den);
}
/* Non‑integral variant (big integer) */
template <typename T,
          typename std::enable_if_t<!std::is_integral<T>::value, int> = 0>
static inline T mul_div(T a, T num, T den)
{
    return a * num / den;
}
#else
/* C version – uses unsigned __int128 intermediate */
static inline seq_t mul_div(seq_t a, seq_t num, seq_t den)
{
    return (seq_t)((unsigned __int128)a * num / den);
}
#endif

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t size = 0;     /* number of cached entries */

    /* initialise cache with a(0) = 1 */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure */
        cache[0] = (seq_t)1;
        size = 1;
    }

    /* extend cache up to n if necessary */
    if (n >= size) {
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return (seq_t)0;    /* allocation failure */
        cache = new_cache;

        for (idx_t i = size; i <= n; ++i) {
            seq_t num = (seq_t)16 * (seq_t)i - (seq_t)12;  /* 16*i – 12 */
            seq_t den = i;                                 /* i */
            cache[i] = mul_div(cache[i-1], num, den);
        }
        size = n + 1;
    }

    return cache[n];
}
