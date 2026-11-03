/*
 * A039463  Numbers whose base-11 representation has the same nonzero number
 *          of 2's and 9's.
 * https://oeis.org/A039463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer whose base‑11 representation
 *            contains an equal nonzero number of digits 2 and 9
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Counts whether a number x has the same non‑zero number of 2’s and 9’s
   in its base‑11 representation.  Returns 1 if true, otherwise 0. */
static int has_equal_2_and_9(seq_t x)
{
    int count2 = 0, count9 = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        int d = (int)(t % (seq_t)11);
        if (d == 2)   ++count2;
        else if (d == 9) ++count9;
        t /= (seq_t)11;
    }
    return (count2 == count9) && (count2 > 0);
}

/* Returns the nth term (1‑based) of the sequence.  The routine scans
   integers in increasing order and counts those that satisfy the
   required property until the desired index is reached.  This
   implementation requires O(n) time and O(1) auxiliary memory, well
   within the 2‑second limit for n up to about 10 000. */
seq_t a(idx_t n)
{
    seq_t candidate = (seq_t)1;
    idx_t  found    = (idx_t)0;

    while (found < n) {
        if (has_equal_2_and_9(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }

    /* Should never be reached. */
    return (seq_t)0;
}
