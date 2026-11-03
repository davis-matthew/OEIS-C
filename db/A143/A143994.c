/*
 * A143994  Years in which there are five Tuesdays in the month of February.
 * https://oeis.org/A143994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = the n‑th year in which February has five Tuesdays.
 *            (Equivalently, the n‑th leap year whose Feb 1 falls on a
 *            Tuesday.)
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline bool is_leap_ymd(uint64_t y)
{
    return ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0);
}

/* Sakamoto's algorithm, Gregorian calendar, 0=Sunday … 6=Saturday */
static int weekday_feb1(uint64_t y)
{
    static const int month[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    int m = 2;          /* February */
    int d = 1;          /* first day */
    if (m <= 2) y -= 1;
    return (int)((y + y / 4 - y / 100 + y / 400 + month[m - 1] + d) % 7);
}

/* ----- helpers ----- */

/* ----- cached sequence ----- */

static seq_t *seq_cache    = NULL;   /* array of cached years */
static size_t seq_cap     = 0;       /* allocated capacity */
static size_t seq_len     = 0;       /* number of cached terms */
static seq_t  last_year   = 0;       /* last year checked for a valid term */

/* ----- public function ----- */

seq_t a(idx_t n)
{
    /* must have n >= 1 (OEIS offset 1) */
    if (n == 0) return 0;   /* defensive, shouldn't happen */

    /* initialise cache on first call */
    if (seq_cache == NULL) {
        seq_cap  = 64;
        seq_len  = 1;
        seq_cache = mem_new(seq_t, seq_cap);
        seq_cache[0] = (seq_t)1752; /* first known term */
        last_year    = (seq_t)1752;
    }

    /* extend cache until we have at least n terms */
    while (seq_len < n) {
        seq_t next_year = last_year + 1;

        /* search forward for the next qualifying year */
        for (;; ++next_year) {
            if (is_leap_ymd((uint64_t)next_year) &&
                weekday_feb1((uint64_t)next_year) == 2) { /* Feb 1 is Tuesday */
                /* add to cache */
                if (seq_len == seq_cap) {
                    seq_cap *= 2;
                    seq_cache = mem_resize(seq_t, seq_cache, seq_cap);
                }
                seq_cache[seq_len++] = next_year;
                last_year = next_year;
                break;
            }
        }
    }

    return seq_cache[n - 1];
}
