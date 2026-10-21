/*
 * A081529  Duplicate of A002944.
 * https://oeis.org/A081529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = lcm(1,2,…,n) / n (which equals A003418(n)/n)
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 22, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cur_n      = 0;
    static seq_t cur_lcm     = 0;

    /* The first call initializes the running lcm to 1. */
    if (cur_n == 0) {
        cur_lcm = (seq_t)1;
    }

    /* If the requested value has already been computed, return it. */
    if (n <= cur_n) {
        return cache[n - 1];
    }

    /* Grow the cache array if necessary. */
    size_t needed = (size_t)n;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();          /* allocation failure */
        cache      = new_arr;
        cache_cap  = new_cap;
    }

    /* Incrementally compute lcm(1..i)/i for i = cur_n+1 … n. */
    for (idx_t i = cur_n + 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t g     = gcd(cur_lcm, i_seq);
        seq_t temp  = cur_lcm / g;     /* lcm(1..i-1)/gcd to avoid overflow */
        cur_lcm     = temp * i_seq;    /* lcm(1..i) */
        cache[i - 1] = cur_lcm / i_seq;/* lcm(1..i)/i */
    }

    cur_n = n;
    return cache[n - 1];
}
