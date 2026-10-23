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
 * Helpers:   easter_is_25
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the Meeus/Jones/Butcher algorithm to compute the Easter date. */
/* The sequence is generated lazily: as many terms as requested are computed and stored in a static buffer. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/easter_is_25.h"

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
                    seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
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
