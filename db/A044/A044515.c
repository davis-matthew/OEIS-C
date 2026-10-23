/*
 * A044515  Numbers n such that string 5,2 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose base‑6 representation contains the
 *            substring “52” but does not contain it in the representation of
 *            (n+1). This implementation performs a linear search with
 *            memoisation to generate each new term only once
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>    /* size_t */

#if defined(__cplusplus)
#   include <vector>
#endif

/* Helper: return true iff the base‑6 representation of x contains the
   consecutive digits 5 and then 2. */
static bool has_52(seq_t x)
{
    const seq_t SIX = (seq_t)6;
    if (x < SIX)
        return false;                    /* less than two digits */

    seq_t prev = x % SIX;                 /* least significant digit */
    x /= SIX;
    while (x != (seq_t)0)
    {
        seq_t d = x % SIX;                /* next digit */
        x /= SIX;
        if (d == (seq_t)5 && prev == (seq_t)2)
            return true;
        prev = d;
    }
    return false;
}

#if defined(__cplusplus)   /* C++ implementation uses std::vector */
static std::vector<seq_t> cache_vec;
static idx_t last_checked = 0;          /* last integer that was examined */

static void cache_grow(size_t new_cap)
{
    if (new_cap <= cache_vec.capacity())
        return;
    cache_vec.reserve(new_cap);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n==0, but harmless */

    /* generate until we have n terms */
    while (cache_vec.size() < (size_t)n)
    {
        ++last_checked;                  /* candidate number */

        if (has_52((seq_t)last_checked) &&
            !has_52((seq_t)(last_checked + (idx_t)1)))
        {
            cache_grow(cache_vec.size() + 1);
            cache_vec.push_back((seq_t)last_checked);
        }
    }
    return cache_vec[n - 1];
}

#else   /* C implementation uses raw malloc/realloc */

static seq_t *cache_arr = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t last_checked = 0;          /* last integer that was examined */

static void cache_grow(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    if (cache_cap == 0)
        cache_cap = 128;
    while (cache_cap < new_cap)
        cache_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache_arr, cache_cap * sizeof(seq_t));
    cache_arr = tmp;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n==0, but harmless */

    /* generate until we have n terms */
    while (cache_len < (size_t)n)
    {
        ++last_checked;                  /* candidate number */

        if (has_52((seq_t)last_checked) &&
            !has_52((seq_t)(last_checked + (idx_t)1)))
        {
            cache_grow(cache_len + 1);
            cache_arr[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache_arr[n - 1];
}

#endif
