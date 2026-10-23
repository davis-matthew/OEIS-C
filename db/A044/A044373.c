/*
 * A044373  Numbers n such that string 4,1 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer m such that the decimal
 *            representation of m contains the substring "41" and the decimal
 *            representation of m-1 does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains41
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains41.h"

/* The main sequence routine */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;      /* array of gathered terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* current capacity of cache */
    static idx_t cur     = 1;        /* next integer to examine */

    /* grow cache if necessary */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t m = (seq_t)cur;
        bool has = contains41(m);
        bool prev_has = false;
        if (cur > 0)
            prev_has = contains41(m - (seq_t)1);

        if (has && !prev_has) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = m;
        }
        cur++;
    }

    return cache[n - 1];
}
