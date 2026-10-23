/*
 * A158581  Numbers having in binary representation at least two ones and two
 *          zeros.
 * https://oeis.org/A158581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having at least two 1-bits and at least two 0-bits
   in their binary representation.  a(1) = 9.  The function returns the
   nth such number.  It scans integers sequentially and checks the
   required bit conditions. */
/* No external utilities are needed; this works for any seq_t that
   supports the usual integer operators (+, &, |, <<, >>). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Scan integers until the nth candidate is found */
    idx_t  count = 0;
    seq_t  cand   = 0;

    while (true)
    {
        /* move to next integer */
        cand++;

        /* Count 1-bits */
        seq_t t   = cand;
        idx_t  ones = 0;
        while (t)
        {
            ones++;
            t &= t - 1;          /* clear least‑significant 1-bit */
        }
        if (ones < 2)
            continue;            /* need at least two ones */

        /* Count total bits needed to represent cand */
        seq_t tmp   = cand;
        idx_t  bits = 0;
        while (tmp)
        {
            bits++;
            tmp >>= 1;
        }
        if (bits == 0)            /* cand == 0 would give bits==0, but we
                                     never consider it (cand starts at 1) */
            bits = 1;

        /* Number of zeros in the representation */
        idx_t zeros = bits - ones;
        if (zeros < 2)
            continue;            /* need at least two zeros */

        /* Found a valid candidate */
        count++;
        if (count == n)
            return cand;
    }
}
