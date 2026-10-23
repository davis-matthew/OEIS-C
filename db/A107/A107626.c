/*
 * A107626  Numbers n such that every digit of both n and n^2 contains a loop
 *          (only digits 0,4,6,8,9 in n and n^2).
 * https://oeis.org/A107626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, dumb
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 20, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   have_loop_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that every digit of n and n² consists only of
   loop digits {0,4,6,8,9}.  The function generates the sequence by
   searching for successive qualifying integers.  It memoises the found
   terms so that calls in arbitrary order are fast. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/have_loop_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n ≠ 0 – return 0 as a safe value */

    /* Static memoisation state */
    static seq_t *terms = NULL;
    static idx_t size   = 0;
    static idx_t cap    = 0;
    static idx_t last   = 0;   /* last integer examined */

    /* Extend the term buffer until we have at least n terms */
    while (size < n) {
        idx_t cur = last + 1;     /* candidate integer */

        /* Fast rejection: check only the digits of cur */
        if (!have_loop_digits((seq_t)cur)) {
            last = cur;
            continue;
        }

        /* Check the square of cur */
        seq_t sq = (seq_t)cur * cur;
        if (!have_loop_digits(sq)) {
            last = cur;
            continue;
        }

        /* cur qualifies – store it */
        if (size == cap) {
            idx_t newcap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
            if (!tmp) abort();     /* allocation failure */
            terms = tmp;
            cap   = newcap;
        }
        terms[size++] = (seq_t)cur;
        last = cur;
    }

    return terms[n - 1];
}
