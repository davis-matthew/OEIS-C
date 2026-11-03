/*
 * A084427  Gregorian calendar years with Ascension Day in April.
 * https://oeis.org/A084427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 38 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Return true if Easter Sunday in 'year' is March 22. */
static bool easter_march22(idx_t year) {
    /* anonymous Gregorian algorithm (Butcher) */
    int a = (int)(year % 19);
    int b = (int)(year / 100);
    int c = (int)(year % 100);
    int d = b / 4;
    int e = b % 4;
    int f = (b + 8) / 25;
    int g = (b - f + 1) / 3;
    int h = (19 * a + b - d - g + 15) % 30;
    int i = c / 4;
    int k = c % 4;
    int l = (32 + 2 * e + 2 * i - h - k) % 7;
    int m = (a + 11 * h + 22 * l) / 451;
    int month = (h + l - 7 * m + 114) / 31;      /* 3 = March, 4 = April */
    int day   = ((h + l - 7 * m + 114) % 31) + 1;

    return month == 3 && day == 22;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_year = 1583;   /* first Gregorian year in common usage */

    while (cache_len <= n) {
        if (easter_march22(next_year)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
