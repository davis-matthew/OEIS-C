/*
 * A039407  Numbers whose base-10 representation has the same nonzero number
 *          of 1's and 7's.
 * https://oeis.org/A039407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A039407
   a(n) = the n-th positive integer whose decimal representation
   contains the same nonzero number of digits 1 and 7.
   (The sequence is defined by a property rather than an explicit formula.) */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Return the n-th element of the sequence.
   n is 1-based; calls may be in any order. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for the offset; return 0 as a sentinel. */
        return (seq_t)0;
    }

    /* Static state for memoisation. */
    static seq_t *seq  = NULL;   /* array of found terms           */
    static size_t cnt  = 0;      /* number of terms stored         */
    static size_t cap  = 0;      /* current capacity of seq array  */
    static idx_t cur   = 1;      /* next integer to test (idx_t)   */

    /* Grow the cache until it contains at least n terms. */
    while (cnt < n) {
        /* Grow the array if necessary. */
        if (cnt == cap) {
            size_t new_cap = (cap == 0) ? 1 : cap << 1;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed; abort.  The harness will treat it
                   as a failure to compute the sequence. */
                abort();
            }
            seq = tmp;
            cap = new_cap;
        }

        /* Count the digits 1 and 7 in the current candidate. */
        unsigned int c1 = 0;
        unsigned int c7 = 0;
        idx_t t = cur;
        if (t == 0) { /* handle the number 0 explicitly (though it never matches) */
            c1 = 0;
            c7 = 0;
        } else {
            while (t > 0) {
                unsigned int d = (unsigned int)(t % 10);
                if (d == 1) ++c1;
                else if (d == 7) ++c7;
                t /= 10;
            }
        }

        /* Store the number if it has equal nonzero counts of 1 and 7. */
        if (c1 == c7 && c1 > 0) {
            seq[cnt++] = (seq_t)cur;
        }

        ++cur;
    }

    /* Return the requested term (cnt ≥ n guarantees this is valid). */
    return seq[n - 1];
}
