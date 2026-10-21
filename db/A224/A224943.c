/*
 * A224943  Non-leap years beginning and ending on Friday.
 * https://oeis.org/A224943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Non-leap years beginning and ending on Friday.
 * Computed by testing each year starting from 1583.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool is_leap(uint64_t y) {
    return (y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0));
}

static int weekday_jan1(uint64_t year) {
    /* Zeller's congruence for Gregorian calendar.
       Returns 0=Sunday, 1=Monday, ..., 6=Saturday. */
    uint64_t y = year;
    int m = 13;          /* January treated as month 13 of previous year */
    y -= 1;
    int d = 1;
    int K = y % 100;
    int J = y / 100;
    int h = (d + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
    int day = (h + 6) % 7;   /* convert to 0=Sunday ... 6=Saturday */
    return day;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static uint64_t next_year = 1583;   /* start searching from 1583 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */
    while (cache_len < n) {
        uint64_t y = next_year;
        if (!is_leap(y) && weekday_jan1(y) == 5) {   /* Friday */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)y;
        }
        next_year++;
    }
    return cache[n - 1];
}
