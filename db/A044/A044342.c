/*
 * A044342  Numbers k such that the string 1,0 occurs in the base 10
 *          representation of k but not of k-1.
 * https://oeis.org/A044342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers k that contain the decimal substring "10" but k-1 does
 *            not
 * Range:     u8 n <= 3, u16 n <= 639, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_substr10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  the function is memoised per problem statement; it works for any
    idx_t (uint64_t) and any seq_t (unsigned integer or arbitrary‑precision
    class).  The implementation uses malloc when compiled as C and
    new/delete when compiled as C++ to avoid undefined behaviour for
    non‑POD types.  Only the C89/C99 and C++17 standard libraries are
    used. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_substr10.h"

/* static memoisation state ---------------------------------------- */
static seq_t *cache      = NULL;   /* array of found terms            */
static size_t cache_len  = 0;      /* number of terms currently cached */
static size_t cache_cap  = 0;      /* capacity of the cache array     */
static idx_t  next_to_check = 1;   /* next candidate integer to test  */
static bool   initialized  = false;

/* enlarge the cache if needed.  The implementation uses 'new' when
   compiled as C++ and 'realloc' when compiled as C. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;

#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
#else
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* initialise static state on first call ----------------------------------- */
static void init_state(void)
{
    if (!initialized) {
        cache_cap = 128;
        /* Allocate the initial cache array */
#if defined(__cplusplus)
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        cache_len = 0;
        next_to_check = 1;
        initialized = true;
    }
}

/* public interface ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, return 0 safely */

    init_state();

    /* generate terms until we have at least n terms cached */
    while (cache_len < n) {
        /* skip numbers until one satisfies the property */
        while (!(
            has_substr10((seq_t)next_to_check) &&
            !has_substr10((seq_t)(next_to_check - 1))))
            ++next_to_check;

        /* store the discovered term */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)next_to_check;
        ++next_to_check;
    }

    return cache[n - 1];
}
