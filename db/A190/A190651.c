/*
 * A190651  Years with exactly one "Friday the 13th", starting from 1901.
 * https://oeis.org/A190651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 997 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A190651: years with exactly one Friday the 13th, starting from 1901.
   This implementation computes the n-th such year by iterating over years
   and counting how many 13th days are Fridays in each year.  Results are cached
   so that arbitrary order of calls up to any n is supported. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/*  Utility: check whether the 13th of a given month falls on Friday
   GB: Zeller's congruence for Gregorian calendar
   h = 0 -> Saturday, … , 6 -> Friday                                         */
static inline bool is_friday_13th(int year, int month)
{
    if (month < 3) {
        month += 12;
        year -= 1;
    }
    int k = year % 100;
    int j = year / 100;
    int h = (13 + (13*(month+1))/5 + k + k/4 + j/4 + 5*j) % 7;
    return h == 6;                /* 6 = Friday */
}

/* Count how many months in the given year have Friday on the 13th. */
static inline int count_friday_13s(int year)
{
    int cnt = 0;
    for (int m = 1; m <= 12; ++m) {
        if (is_friday_13th(year, m))
            ++cnt;
    }
    return cnt;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*  Cache holding the first years with exactly one Friday the 13th   */
static seq_t *year_cache = NULL;      /* array of results          */
static size_t cache_size = 0;         /* number of stored elements */
static size_t cache_cap  = 0;         /* current allocated space   */
static int last_year = 1900;          /* last year examined         */

/* Ensure the cache has at least 'needed' capacity. */
static void expand_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;
    seq_t *tmp = mem_resize(seq_t, year_cache, newcap);
    year_cache = tmp;
    cache_cap = newcap;
}

/* ------------------------------------------------------------ */
/*  Public sequence function
   seq_t a(idx_t n): returns the n-th year (1‑based) with exactly one
   Friday the 13th.                                   */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset is 1, ignore n==0 */

    size_t target = (size_t)n;

    /* Fill cache until we have at least 'target' elements */
    while (cache_size < target) {
        ++last_year;
        if (count_friday_13s(last_year) == 1) {
            expand_cache(cache_size + 1);
            year_cache[cache_size] = (seq_t)last_year;
            ++cache_size;
        }
    }

    return year_cache[target - 1];
}
