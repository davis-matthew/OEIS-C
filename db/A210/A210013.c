/*
 * A210013  Pandigital numbers (A050278) with each product of three adjacent
 *          digits visible as a substring of the digits.
 * https://oeis.org/A210013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 4, big any n
 * Helpers:   array_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search over all 10! permutations of the digits
   0..9.  For each permutation, we test whether every product of three
   consecutive digits occurs as a contiguous substring of the 10‑digit
   number.  The qualifying numbers are collected, sorted, and cached.
   Subsequent calls simply index into the cached array. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/array_to_seq.h"

/* Forward declaration of the required API. */
seq_t a(idx_t n);

/* Compare two seq_t values for qsort. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Test whether the 10‑digit array d satisfies the substring‑product property. */
static bool satisfies_property(const int *d)
{
    for (int i = 0; i <= 7; ++i)  /* 8 products of three adjacent digits */
    {
        int prod = d[i] * d[i + 1] * d[i + 2];
        /* Determine decimal length of prod. */
        int len = (prod >= 100) ? 3 : (prod >= 10) ? 2 : 1;
        bool found = false;
        for (int start = 0; start <= 10 - len; ++start)
        {
            int sub = 0;
            for (int k = 0; k < len; ++k)
                sub = sub * 10 + d[start + k];
            if (sub == prod) { found = true; break; }
        }
        if (!found) return false;
    }
    return true;
}

/* Recursive generation of all permutations of digits 0..9. */
static void generate_perm(int pos, uint16_t used, int *curr,
                          seq_t **out, size_t *len, size_t *cap)
{
    if (pos == 10)
    {
        if (satisfies_property(curr))
        {
            if (*len == *cap)
            {
                *cap = (*cap == 0) ? 128 : *cap * 2;
                *out = (seq_t *)realloc(*out, (*cap) * sizeof(seq_t));
            }
            (*out)[(*len)++] = array_to_seq(curr);
        }
        return;
    }
    for (int d = 0; d <= 9; ++d)
    {
        if (used & (1 << d)) continue;
        if (pos == 0 && d == 0) continue;      /* avoid leading zero */
        curr[pos] = d;
        generate_perm(pos + 1, used | (1 << d), curr, out, len, cap);
    }
}

/* Initialise the cached array of all valid numbers. */
static void init_cache(seq_t **cache, size_t *len, size_t *cap)
{
    int digits[10];
    generate_perm(0, 0, digits, cache, len, cap);
    qsort(*cache, *len, sizeof(seq_t), cmp_seq);
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialise cache on first call. */
    if (cache == NULL)
        init_cache(&cache, &cache_len, &cache_cap);

    if (n == 0 || n > cache_len) return (seq_t)0;
    return cache[n - 1];
}
