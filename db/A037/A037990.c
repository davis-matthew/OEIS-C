/*
 * A037990  Numbers whose maximal base 7 run length is 3.
 * https://oeis.org/A037990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑7 representation has
 *            maximal consecutive digit run length exactly 3. The sequence is
 *            obtained by scanning the natural numbers and keeping those whose
 *            base‑7 representation contains at most one run of length 3 and
 *            no longer runs
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
/* In C++ builds seq_t may be an arbitrary‑precision integer class,
   so use std::vector for dynamic storage. */
static std::vector<seq_t> cache;
#else
/* In C builds seq_t is a plain integer type; use simple dynamic array. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
#endif

/* next natural number to test */
static seq_t next_num = 1;

/* Ensure the cache in C builds can hold at least 'need' elements. */
#ifndef __cplusplus
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* memory failure */
    cache = new_cache;
    cache_cap = new_cap;
}
#endif

/* Test whether a number has base‑7 maximum run length exactly 3. */
static bool has_run_length_three(seq_t x)
{
    int max_run = 0;
    int cur_run = 0;
    int prev   = -1;

    while (x > 0)
    {
        int d = (int)(x % 7);
        x   /= 7;

        if (prev == -1)
        {
            prev   = d;
            cur_run = 1;
        }
        else if (d == prev)
        {
            ++cur_run;
        }
        else
        {
            if (cur_run > max_run) max_run = cur_run;
            prev    = d;
            cur_run = 1;
        }
    }
    if (cur_run > max_run) max_run = cur_run;
    return max_run == 3;
}

/* Extend the cached sequence until it contains at least 'target' terms. */
static void extend_cache(idx_t target)
{
#ifdef __cplusplus
    while ((idx_t)cache.size() < target)
    {
        while (!has_run_length_three(next_num))
            ++next_num;
        cache.push_back(next_num);
        ++next_num;
    }
#else
    while (cache_size < (size_t)target)
    {
        while (!has_run_length_three(next_num))
            ++next_num;
        ensure_capacity(cache_size + 1);
        cache[cache_size++] = next_num;
        ++next_num;
    }
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1, but safe */
    extend_cache(n);
    return cache[n - 1];
}
