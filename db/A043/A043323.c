/*
 * A043323  Numbers having three 0's in base 3.
 * https://oeis.org/A043323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: generates a(n), the nth positive integer whose base‑3
 * representation contains exactly three zeros.  The function enumerates
 * candidates by brute‑force scanning upwards and caches the results so
 * subsequent calls are fast. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of found numbers      */
static idx_t  cache_len = 0;         /* number of elements stored  */
static idx_t  cache_cap = 0;         /* capacity of the array      */
static idx_t  next_test = 1;         /* next number to test        */

/* Ensure that cache contains at least n entries. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        /* Grow storage if necessary */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Count zeros in base‑3 representation of next_test */
        idx_t x = next_test;
        int zero_cnt = 0;
        while (x > 0) {
            if ((x % 3) == 0) ++zero_cnt;
            x /= 3;
        }

        if (zero_cnt == 3) {
            cache[cache_len++] = (seq_t)next_test;
        }

        ++next_test;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behaviour below offset irrelevant */
    if (n > cache_len) ensure_cache(n);
    return cache[n - 1];
}
