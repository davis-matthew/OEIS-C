/*
 * A044248  Numbers k such that string 7,5 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that its base‑8 representation *
 *            contains the substring "75" while the integer k−1 does not
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard C/C++ headers */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Test whether a number contains the octal substring 75 */
static bool has_75(seq_t v)
{
    bool first = true;
    seq_t prev = 0;
    while (v != 0)
    {
        seq_t d = v % (seq_t)8;       /* least significant octal digit */
        if (!first && prev == 5 && d == 7)
            return true;
        prev = d;
        first = false;
        v /= (seq_t)8;
    }
    return false;
}

#ifdef __cplusplus
/* C++ implementation – use std::vector */
static std::vector<seq_t> cache_vec;
static seq_t next_start = 1;

static void extend_cache(idx_t target)
{
    while ((idx_t)cache_vec.size() < target)
    {
        seq_t k = next_start;
        for (;;)
        {
            if (has_75(k) && !has_75(k - 1))
            {
                cache_vec.push_back(k);
                next_start = k + 1;
                break;
            }
            ++k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n < 1 – return 0 */
    extend_cache(n);
    return cache_vec[(size_t)(n - 1)];
}
#else
/* C implementation – dynamic array with malloc/realloc */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_start = 1;

static void extend_cache(idx_t target)
{
    while (cache_len < target)
    {
        seq_t k = next_start;
        for (;;)
        {
            if (has_75(k) && !has_75(k - 1))
            {
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();   /* memory exhaustion */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                next_start = k + 1;
                break;
            }
            ++k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n < 1 – return 0 */
    extend_cache(n);
    return cache[n - 1];
}
#endif
