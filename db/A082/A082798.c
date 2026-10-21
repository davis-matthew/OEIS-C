/*
 * A082798  Smallest multiple of n beginning with 8.
 * https://oeis.org/A082798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For a given n, find the smallest multiple that starts
 * with decimal digit 8.  For each number of digits d = 0,1,2,… compute
 * the interval [8·10^d , 9·10^d) and choose the smallest multiple
 * of n that falls inside it.  The first d that succeeds gives the
 * desired value.  All arithmetic is performed with seq_t, so the
 * routine works for arbitrary‑precision and for 8/16/32/64‑bit builds. */

#include <stdbool.h>

/* Return the smallest multiple of n whose decimal representation
   begins with the digit 8.  n ≥ 1. */
seq_t a(idx_t n)
{
    /* cast once – seq_t arithmetic is done on this value */
    const seq_t sn = (seq_t)n;

    /* current 10^d, starts at 1 (10^0) */
    seq_t pow10 = 1;

    for (int d = 0; /* d increases */; ++d) {
        /* low = 8·10^d     high = 9·10^d   (high is exclusive) */
        const seq_t low  = pow10 * (seq_t)8;
        const seq_t high = pow10 * (seq_t)9;

        /* smallest k with sn*k >= low : ceil(low / sn) */
        seq_t k = low / sn;
        if (low % sn) {
            ++k;          /* correct ceil if division non‑exact */
        }

        const seq_t val = k * sn;

        /* if the value lies inside the interval, it is the answer */
        if (val < high) {
            return val;
        }

        /* next power of 10 for next iteration */
        /* guard against potential overflow for very large seq_t:
           if pow10 is already huge, multiplication cannot overflow
           because seq_t is of arbitrary precision. */
        pow10 *= (seq_t)10;
    }
}
