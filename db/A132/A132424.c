/*
 * A132424  a(0)=1 and, for n>0, a(n)=a(Floor((n-1)/a[n-1]))+2.
 * https://oeis.org/A132424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1087, u16 n <= 8898, u32 n <= 8898, u64 n <= 8898,
 *            u128 n <= 8898, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 1 if n = 0,
    a(n) = a( floor( (n-1)/a(n-1) ) ) + 2      (n > 0)    */
/*  The function caches computed values so that any n can be
    queried in any order.  It works for both the plain integer
    builds (C) and the arbitrary‑precision build (C++).    */

#include <stddef.h>    /* size_t */
#include <stdlib.h>    /* malloc, realloc, free, abort */

#ifdef __cplusplus
#include <vector>
using std::vector;
#endif

/* Forward declarations */
static seq_t a_rec(idx_t n);

/* ------------------------------------------------------------------ */
/*  Cache handling – the two compilation modes are handled separately. */
/* ------------------------------------------------------------------ */

#ifdef __cplusplus  /* C++ build – seq_t is a class */

static vector<seq_t> cache_vec;

/* Ensure cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_vec.size() > n) return;
    cache_vec.resize(n + 1);   /* default constructs new elements */
}

#else   /* C build – seq_t is an integer type */

static seq_t *cache_arr = NULL;   /* array of cached values */
static size_t cache_cap = 0;      /* allocated size */

/* Ensure cache can hold indices up to n (inclusive).  Newly
   allocated elements are zeroed (which is correct for integer
   types).  */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;

    size_t needed = n + 1;
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* memory allocation failure */

    if (new_cap > cache_cap) {
        /* zero the new portion */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    }

    cache_arr = tmp;
    cache_cap = new_cap;
}

#endif

/* ------------------------------------------------------------------ */
/*  Recursive computation with memoisation                               */
/* ------------------------------------------------------------------ */

static seq_t a_rec(idx_t n)
{
    ensure_cache(n);

#ifdef __cplusplus
    seq_t *cache = cache_vec.data();
#else
    seq_t *cache = cache_arr;
#endif

    /* Already computed ? */
    if (cache[n] != 0) return cache[n];

    if (n == 0) {
        cache[0] = (seq_t)1;
        return cache[0];
    }

    /* a(n-1) must be known first */
    seq_t prev = a_rec(n - 1);

    /* floor( (n-1)/prev ) – both operands are idx_t arithmetic.  */
    idx_t idx = (idx_t)((seq_t)(n - 1) / prev);

    /* a(n) = a(idx) + 2 */
    cache[n] = a_rec(idx) + (seq_t)2;
    return cache[n];
}

/* ------------------------------------------------------------------ */
/*  Public interface                                                     */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    return a_rec(n);
}
