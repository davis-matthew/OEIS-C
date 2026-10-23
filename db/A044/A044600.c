/*
 * A044600  Numbers n such that string 4,0 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has40oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑8 representation contains "40" but whose successor does not. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has40oct.h"

/* main sequence accessor */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* dynamic array of found terms      */
    static size_t cache_len = 0;     /* number of terms stored           */
    static size_t cache_cap = 0;     /* allocated capacity                */
    static uint64_t cur = 0;         /* next value to test                */

    if (cache == NULL) {             /* first call: initialise space      */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);  /* guard against OOM */
    }

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* find next integer satisfying the definition */
        do {
            ++cur;
        } while (!(has40oct(cur) && !has40oct(cur + 1)));

        /* grow storage if necessary */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) {
                free(cache);
                exit(1);
            }
            cache = tmp;
        }

        cache[cache_len++] = (seq_t)cur;
    }

    return cache[n - 1];
}
