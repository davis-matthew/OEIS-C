/*
 * A044427  Numbers n such that string 9,5 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044427: Numbers n such that the decimal representation of n
 * contains the substring "95" but n-1 does not. This function searches
 * for such numbers incrementally and stores the results in a dynamic
 * cache so each term can be returned in O(1) after it has been computed. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: true iff x contains the consecutive digits 9 followed by 5 */
static bool has95(seq_t x)
{
    int prev = -1;                  /* higher digit seen so far */
    while (x > 0)
    {
        int d = (int)(x % 10);      /* current lower digit */
        x /= 10;
        if (prev == 5 && d == 9)    /* 9 (higher) 5 (lower) */
            return true;
        prev = d;
    }
    return false;
}

/* Cache for already found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 95;   /* number to test next */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* sequence is 1‑indexed; undefined for n=0 */

    /* Generate terms until we have at least n of them. */
    if (cache_len < (size_t)n)
    {
        if (cache_len == 0)
            next_candidate = 95;
        else
            next_candidate = cache[cache_len - 1] + 1;

        while (cache_len < (size_t)n)
        {
            seq_t cand = next_candidate;
            if (has95(cand) && !has95(cand - 1))
            {
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = cand;
            }
            next_candidate++;
        }
    }

    return cache[n - 1];
}
