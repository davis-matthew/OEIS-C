/*
 * A044724  Numbers n such that string 1,1 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044724: numbers n such that the decimal representation of n
 * contains the substring "11" but the representation of n+1 does NOT. */

/* The algorithm scans integers one by one, keeps a cache of all values
 * found so far, and grows the cache lazily.  The cache implementation
 * is special‑cased for C++ builds where seq_t may be an arbitrary–precision
 * class. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether the decimal representation of x contains "11". */
static bool has_11(seq_t x)
{
    while (x >= 10)
    {
        if (x % 100 == 11)
            return true;
        x /= 10;
    }
    return false;
}

/* Cached data.  The static variables exist for all calls and allow
 * the function to return any requested index in any order. */
static seq_t *cache      = NULL;   /* Array of found terms */
static size_t  cap       = 0;     /* Allocated capacity */
static size_t  size_     = 0;     /* Number of terms stored */
static seq_t  last_checked = 0;   /* Largest integer examined so far */

/* Ensure that the cache has space for at least min_cap elements. */
static void ensure_capacity(size_t min_cap)
{
    if (cap >= min_cap)
        return;

    size_t new_cap = cap ? cap << 1 : 4;
    while (new_cap < min_cap)
        new_cap <<= 1;

#ifdef __cplusplus
    /* For C++ builds where seq_t may be a class type we use operator new
     * and copy each element explicitly. */
    seq_t *new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < size_; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
#else
    /* For C builds the cast from void* to seq_t* is valid. */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for 0; OEIS offset is 1 */

    /* Lazily compute terms until we have at least n values. */
    while (size_ < (size_t)n)
    {
        ++last_checked;

        if (has_11(last_checked) && !has_11(last_checked + 1))
        {
            ensure_capacity(size_ + 1);
            cache[size_++] = last_checked;
        }
    }

    return cache[(size_t)n - 1];
}
