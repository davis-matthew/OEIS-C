/*
 * A044336  Numbers n such that string 0,4 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044336: numbers n such that the decimal representation of
 * n contains the substring "04" but the representation of n-1 does not.
 * The following implementation generates the sequence on demand,
 * caching the results so repeated calls are fast.
 * It works for any unsigned integer type seq_t (including
 * arbitrary–precision classes) and handles any order of calls.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Cached terms of the sequence */
static seq_t *cache = NULL;    /* array of terms */
static size_t cache_len = 0;   /* number of terms cached */
static size_t cache_cap = 0;   /* allocated capacity */

/* Returns true if the decimal representation of x contains "04". */
static bool contains04(seq_t x)
{
    if (x == 0) return false;
    int prev = -1;            /* digit seen in the next higher position */
    while (x != 0) {
        int d = (int)(x % 10);       /* least significant digit */
        if (prev == 4 && d == 0) return true;
        prev = d;
        x = x / 10;
    }
    return false;
}

/* Ensure that the cache can hold at least 'needed' terms. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* OEIS sequence A044336.  a(1) is the first term. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* 0 is outside the sequence range */
    size_t target = (size_t)n;

    if (target > cache_len) {
        /* Start searching from the number after the last cached term. */
        seq_t cur = cache_len ? cache[cache_len - 1] + 1 : 1;

        grow_cache(target);          /* make sure we have enough space */

        while (cache_len < target) {
            if (contains04(cur) && !contains04(cur - 1)) {
                cache[cache_len++] = cur;
            }
            ++cur;
        }
    }
    return cache[target - 1];
}
