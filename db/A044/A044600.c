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
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑8 representation contains "40" but whose successor does not. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether v written in base 8 contains the substring 40 */
static bool has40oct(uint64_t v) {
    uint64_t prev = 8;               /* sentinel larger than any octal digit */
    while (v) {
        uint64_t d = v % 8;
        if (prev == 0 && d == 4) return true;  /* higher digit 4, lower digit 0 */
        prev = d;
        v /= 8;
    }
    return false;
}

/* main sequence accessor */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* dynamic array of found terms      */
    static size_t cache_len = 0;     /* number of terms stored           */
    static size_t cache_cap = 0;     /* allocated capacity                */
    static uint64_t cur = 0;         /* next value to test                */

    if (cache == NULL) {             /* first call: initialise space      */
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
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
            seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
            if (tmp == NULL) {
                mem_free(cache);
                exit(1);
            }
            cache = tmp;
        }

        cache[cache_len++] = (seq_t)cur;
    }

    return cache[n - 1];
}
