/*
 * A157120  Positive numbers y such that y^2 is of the form x^2+(x+103)^2 with
 *          integer x.
 * https://oeis.org/A157120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6, with a(1)=73, a(2)=103,
 *            a(3)=205, a(4)=233, a(5)=515, a(6)=1157
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
# define SEQ_CONST(v) static_cast<seq_t>(v)
#else
# define SEQ_CONST(v) ((seq_t)(v))
#endif

/* Cache for computed terms */
static seq_t* cache = NULL;
static idx_t  cache_cap = 0;   /* allocated size in terms of seq_t elements */
static idx_t  computed  = 0;   /* number of terms already populated in cache */
static bool   init_done = false;

/* Ensure the cache can hold at least 'needed' terms */
static void
ensure_cache_capacity(idx_t needed)
{
    if (needed > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap < needed) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Sequence function */
seq_t
a(idx_t n)
{
    /* Base cases: direct lookup */
    if (n <= 6) {
        static const seq_t init[6] = {
            SEQ_CONST(73), SEQ_CONST(103), SEQ_CONST(205),
            SEQ_CONST(233), SEQ_CONST(515), SEQ_CONST(1157)
        };
        return init[n - 1];
    }

    /* Prepare cache and seed first six terms */
    if (!init_done) {
        ensure_cache_capacity(6);
        static const seq_t init[6] = {
            SEQ_CONST(73), SEQ_CONST(103), SEQ_CONST(205),
            SEQ_CONST(233), SEQ_CONST(515), SEQ_CONST(1157)
        };
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        computed = 6;
        init_done = true;
    }

    /* Compute up to desired n */
    if (n > computed) {
        ensure_cache_capacity(n);
        for (idx_t i = computed; i < n; ++i) {
            cache[i] = (seq_t)6 * cache[i - 3] - cache[i - 6];
        }
        computed = n;
    }

    return cache[n - 1];
}
