/*
 * A134844  Numbers k such that k contains no zero but k^2 contains at least
 *          one zero.
 * https://oeis.org/A134844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 53, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134844:
 * numbers k such that k has no zero digit, and k^2 contains
 * at least one zero digit.
 * The algorithm searches numbers in increasing order, storing
 * found values so that later calls can be answered quickly.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: true if x contains any digit 0 */
static inline bool has_zero_digit(seq_t x)
{
    seq_t t = x;
    while (t > 0) {
        if ((t % 10) == 0)
            return true;
        t /= 10;
    }
    return false;
}

/* helper: true if x has no digit 0 */
static inline bool no_zero_digits(seq_t x)
{
    seq_t t = x;
    while (t > 0) {
        if ((t % 10) == 0)
            return false;
        t /= 10;
    }
    return true;
}

/* external function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for 0 by sequence definition */

    /* static storage for memoisation */
    static seq_t *terms = NULL;    /* array of found terms  */
    static size_t size   = 0;      /* number of terms stored       */
    static size_t cap    = 0;      /* capacity of the array        */
    static seq_t last_checked = (seq_t)0; /* last candidate examined */

    /* grow array if necessary and find terms until we have n */
    while (size < n) {
        /* extend capacity */
        if (size == cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!new_arr) {
                /* allocation failure – abort; framework may not handle this fine */
                abort();
            }
            terms = new_arr;
            cap = new_cap;
        }

        /* search next eligible candidate */
        seq_t cand = last_checked + (seq_t)1;
        for (;;) {
            if (no_zero_digits(cand)) {
                seq_t sq = cand * cand;
                if (has_zero_digit(sq)) {
                    terms[size++] = cand;
                    last_checked = cand;
                    break;  /* found one term: break inner loop */
                }
            }
            last_checked = cand;
            ++cand;
        }
    }

    return terms[n - 1];
}
