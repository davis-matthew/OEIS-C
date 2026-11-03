/*
 * A013983  Expansion of 1/(1-x^2-x^3-x^4-x^5-x^6).
 * https://oeis.org/A013983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6)+a(n-5)+a(n-4)+a(n-3)+a(n-2) (n ≥ 6) Initial
 *            values: a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=3
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 51, u64 n <= 100,
 *            u128 n <= 199, big any n
 * Validated: standard, pass, 53 terms through n = 943 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of the cache */
    static idx_t  len   = 0;   /* number of values stored (next free index) */

    /* initialise the cache on the first call */
    if (!cache) {
        /* initial capacity: enough for the first 6 values */
        grow_seq(&cache, &cap, 6);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)3;
        len = 6;
    }

    /* extend cache up to n (inclusive) if necessary */
    while (len <= n) {
        /* ensure there is room for one more element */
        if (len >= cap) {
            grow_seq(&cache, &cap, len + 1);
        }

        /* compute the next term using the recurrence;
           all indices used are < len and ≥ 0 by construction */
        seq_t sum = cache[len - 2] + cache[len - 3] +
                    cache[len - 4] + cache[len - 5] + cache[len - 6];
        cache[len] = sum;
        ++len;
    }

    return cache[n];
}
