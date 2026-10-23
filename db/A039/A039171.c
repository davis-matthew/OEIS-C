/*
 * A039171  Numbers whose base-11 representation has the same number of 1's
 *          and 8's.
 * https://oeis.org/A039171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑11 representation has the same number of 1's and 8's. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper that counts the digits 1 and 8 in the base‑11 representation of x. */
static void count_1_and_8(seq_t x, unsigned *c1, unsigned *c8)
{
    unsigned ones  = 0;
    unsigned eights = 0;
    while (x != (seq_t)0)
    {
        seq_t d = x % (seq_t)11u;
        if (d == (seq_t)1u)
            ++ones;
        else if (d == (seq_t)8u)
            ++eights;
        x = x / (seq_t)11u;
    }
    *c1  = ones;
    *c8  = eights;
}

#ifdef __cplusplus
/* C++ implementation using std::vector for dynamic allocation */
#include <vector>
static std::vector<seq_t> cache;
static seq_t last_checked = 0;

seq_t a(idx_t n)
{
    if (cache.empty())
        cache.reserve(16384);          /* initial capacity */

    /* extend cache to hold at least n elements */
    if ((size_t)n > cache.capacity())
    {
        size_t cap = cache.capacity();
        if (cap == 0) cap = 16384;
        while (cap < (size_t)n) cap <<= 1;
        cache.reserve(cap);
    }

    /* generate terms until we have n of them */
    while (cache.size() < (size_t)n)
    {
        unsigned c1, c8;
        count_1_and_8(last_checked, &c1, &c8);
        if (c1 == c8)
            cache.push_back(last_checked);
        ++last_checked;
    }
    return cache[(size_t)n - 1];
}
#else
/* C implementation using malloc/realloc */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* capacity of the array */
static seq_t last_checked = 0;

static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16384;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (cache == NULL)
        ensure_capacity(16384);   /* initialise the array */

    /* generate terms until the required index is present */
    while (cache_len < (size_t)n)
    {
        unsigned c1, c8;
        count_1_and_8(last_checked, &c1, &c8);
        if (c1 == c8)
            cache[cache_len++] = last_checked;
        ++last_checked;
    }
    return cache[(size_t)n - 1];
}
#endif
