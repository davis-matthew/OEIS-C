/*
 * A104019  Years in the Gregorian calendar for which Easter falls on the 25th
 *          day of the month.
 * https://oeis.org/A104019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th Gregorian year (≥1582) in which Easter falls on
 *            the 25th day of the month
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the Meeus/Jones/Butcher algorithm to compute the Easter date. */
/* The sequence is generated lazily: as many terms as requested are computed and stored in a static buffer. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Determine whether Easter in the given Gregorian year falls on the 25th day. */
static int easter_is_25(idx_t year_idx)
{
    uint64_t y = (uint64_t)year_idx;               /* use 64‑bit arithmetic to be safe */
    uint64_t a = y % 19;
    uint64_t b = y / 100;
    uint64_t c = y % 100;
    uint64_t d = b / 4;
    uint64_t e = b % 4;
    uint64_t f = (b + 8) / 25;
    uint64_t g = (b - f + 1) / 3;
    uint64_t h = (19 * a + b - d - g + 15) % 30;
    uint64_t i = c / 4;
    uint64_t k = c % 4;
    uint64_t l = (32 + 2 * e + 2 * i - h - k) % 7;
    uint64_t m = (a + 11 * h + 22 * l) / 451;
    /* month = 3 (March) or 4 (April) */
    uint64_t month = (h + l - 7 * m + 114) / 31;
    uint64_t day   = ((h + l - 7 * m + 114) % 31) + 1;
    return (day == 25);
}

/* External linkage function required by the build system. */
seq_t a(idx_t n)
{
    /* Static storage for generated terms. */
    static seq_t *list = NULL;   /* array of years */
    static size_t len   = 0;     /* number of terms stored */
    static size_t cap   = 0;     /* capacity of the array */
    static idx_t last_year = 1581; /* last year examined (start from 1582) */

    if (n == 0) return (seq_t)0; /* behaviour below offset is unspecified */

    /* Grow the array as needed. */
    if (len < (size_t)n) {
        /* Ensure capacity to store at least n terms. */
        while (len < (size_t)n) {
            /* Extend search until we find the nth term. */
            last_year++;
            if (easter_is_25(last_year)) {
                if (len >= cap) {
                    /* Grow by doubling or start at 64. */
                    size_t new_cap = (cap == 0) ? 64 : cap * 2;
                    seq_t *tmp = mem_resize(seq_t, list, new_cap);
                    if (!tmp) exit(EXIT_FAILURE);
                    list = tmp;
                    cap  = new_cap;
                }
                list[len] = (seq_t)last_year;
                len++;
            }
        }
    }
    return list[n - 1];
}
