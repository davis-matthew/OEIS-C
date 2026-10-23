/*
 * A044963  Numbers with no two equally numerous base 14 digits.
 * https://oeis.org/A044963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: Numbers with no two equally numerous base 14 digits
 * a(1) = 1, a(2) = 2, …, a(14) = 15, a(15) = 30, a(16) = 45, …
 *
 * The function returns the n-th term in the sequence (a(1) is first).
 *
 * Implementation: iterate natural numbers, test the digit‑frequency
 * property, and memoise the found terms so repeated calls are fast.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- helpers for dynamic array handling ---------- */
static seq_t *allocate_array(size_t cap)
{
#if defined(__cplusplus)
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t *reallocate_array(seq_t *ptr, size_t old_cap, size_t new_cap)
{
#if defined(__cplusplus)
    seq_t *new_arr = new seq_t[new_cap];
    if (ptr)
    {
        size_t i, n = old_cap < new_cap ? old_cap : new_cap;
        for (i = 0; i < n; ++i)
            new_arr[i] = ptr[i];
        delete[] ptr;
    }
    return new_arr;
#else
    return (seq_t *)realloc(ptr, new_cap * sizeof(seq_t));
#endif
}

static void ensure_capacity(size_t needed)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = reallocate_array(cache, cache_cap, new_cap);
    cache = new_arr;
    cache_cap = new_cap;
}

/* ---------- digit‑frequency test ---------- */
static bool good(seq_t x)
{
    unsigned char freq[14] = {0};
    seq_t tmp = x;

    while (tmp > (seq_t)0)
    {
        unsigned r = (unsigned)(tmp % (seq_t)14);
        freq[r]++;
        tmp = tmp / (seq_t)14;
    }

    for (int i = 0; i < 14; ++i)
    {
        if (freq[i] == 0)
            continue;
        for (int j = i + 1; j < 14; ++j)
        {
            if (freq[j] == 0)
                continue;
            if (freq[i] == freq[j])
                return false;
        }
    }
    return true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t candidate = 1; /* start from 1 */

    if (cache_len < (size_t)n)
    {
        ensure_capacity((size_t)n);
        cache = allocate_array(cache_cap); /* set after ensure_capacity */
        /* note: ensure_capacity may have reallocated; we keep cache updated after */
        while (cache_len < (size_t)n && candidate > (seq_t)0)
        {
            if (good(candidate))
            {
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *new_arr =
                        reallocate_array(cache, cache_cap, new_cap);
                    cache = new_arr;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
            }
            candidate = candidate + (seq_t)1;
            if (candidate == (seq_t)0) /* overflow, stop */
                break;
        }
    }

    /* If we didn't find enough terms (overflow case), return 0 */
    if (cache_len < (size_t)n)
        return (seq_t)0;

    return cache[n - 1];
}
