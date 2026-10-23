/*
 * A021074  Expansion of 1/((1-x)(1-2x)(1-4x)(1-5x)).
 * https://oeis.org/A021074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 12*a(n-1) - 49*a(n-2) + 78*a(n-3) - 40*a(n-4)
 * with initial values a(0)=1, a(1)=12, a(2)=95, a(3)=630.
 */
#include <stdlib.h>

#if defined(__cplusplus)
# include <type_traits>
/* Multiply Alex by a small unsigned constant using 128‑bit intermediate
   when possible.  For user‑defined big‑int types the normal arithmetic
   operators are used. */
static inline seq_t multiply_int(const seq_t a, unsigned int c)
{
    if constexpr (std::is_arithmetic_v<seq_t>) {
        return (seq_t)((unsigned __int128)c * (unsigned __int128)a);
    } else {
        return (seq_t)(c * a);
    }
}
#else
/* In plain C we rely on the __int128 extension (available on GCC/Clang). */
static inline seq_t multiply_int(const seq_t a, unsigned int c)
{
    return (seq_t)((unsigned __int128)c * (unsigned __int128)a);
}
#endif

seq_t a(idx_t n)
{
    /* Static dynamic cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* initialise cache with first four terms */
    if (cache == NULL) {
        size_t init_cap = 4;
        cap = init_cap;
        len = init_cap;
#if defined(__cplusplus)
        /* Use C++ new[] for possible big‑int types. */
        cache = new seq_t[cap];
        if (!cache) return 0;  /* unlikely in test harness */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)95;
        cache[3] = (seq_t)630;
#else
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return 0;  /* unlikely in test harness */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)95;
        cache[3] = (seq_t)630;
#endif
    }

    /* grow cache if necessary */
    size_t target = (size_t)n + 1;
    if (target > cap) {
        size_t new_cap = cap ? cap : 1;
        while (target > new_cap)
            new_cap <<= 1;   /* double until sufficient */
#if defined(__cplusplus)
        seq_t *new_buf = new seq_t[new_cap];
        for (size_t i = 0; i < cap; ++i)
            new_buf[i] = cache[i];
        delete[] cache;
        cache = new_buf;
#else
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf) return 0;  /* unlikely in test harness */
        /* zero the new part */
        for (size_t i = cap; i < new_cap; ++i)
            new_buf[i] = (seq_t)0;
        cache = new_buf;
#endif
        cap = new_cap;
    }

    /* compute terms up to n */
    for (size_t i = len; i < target; ++i) {
        seq_t val;
        if (i < 4) {
            val = cache[i];
        } else {
            val = multiply_int(cache[i-1], 12);
            val -= multiply_int(cache[i-2], 49);
            val += multiply_int(cache[i-3], 78);
            val -= multiply_int(cache[i-4], 40);
        }
        cache[i] = val;
    }
    len = target;
    return cache[n];
}
