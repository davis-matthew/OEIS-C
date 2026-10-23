/*
 * A043094  Every string of 2 consecutive base 8 digits contains exactly 2
 *          distinct numbers.
 * https://oeis.org/A043094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth nonnegative integer whose base‑8 representation
   contains no consecutive equal digits (i.e., every pair of adjacent octal
   digits differs).  Computed by scanning integers in increasing order. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t  x = 0;
    const seq_t oct8 = (seq_t)8;

    for (;;) {
        /* Check whether x has no two consecutive equal octal digits */
        seq_t v = x;
        if (v == (seq_t)0) {                     /* special case: 0 */
            /* Valid */
            if (count == n) return x;
            ++count;
        } else {
            seq_t prev = v % oct8;
            v /= oct8;
            int ok = 1;
            while (v != (seq_t)0) {
                seq_t d = v % oct8;
                if (d == prev) { ok = 0; break; }
                prev = d;
                v /= oct8;
            }
            if (ok) {
                if (count == n) return x;
                ++count;
            }
        }
        ++x;
    }
}
