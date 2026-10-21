/*
 * A084427  Gregorian calendar years with Ascension Day in April.
 * https://oeis.org/A084427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   easter_march22
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084427 – Gregorian calendar years with Ascension Day in April.
   A year has Ascension in April iff Easter Sunday falls on March 22.
   The algorithm below computes Easter using the Anonymous Gregorian
   Algorithm (Butcher) and tests for the March 22 condition. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/easter_march22.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_year = 1583;   /* first Gregorian year in common usage */

    while (cache_len <= n) {
        if (easter_march22(next_year)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_year;
        }
        ++next_year;
    }
    return cache[n];
}
