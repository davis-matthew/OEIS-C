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
 * Helpers:   is_leap_ymd, weekday_feb1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_leap_ymd.h"
#include "../utils/weekday_feb1.h"

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
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
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
                    seq_cache = (seq_t *)realloc(seq_cache,
                                                seq_cap * sizeof(seq_t));
                }
                seq_cache[seq_len++] = next_year;
                last_year = next_year;
                break;
            }
        }
    }

    return seq_cache[n - 1];
}
