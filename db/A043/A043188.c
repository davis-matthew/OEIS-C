/*
 * A043188  Numbers k such that 1 and 2 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A043188.
   The function counts integers k in ascending order and returns the
   n‑th integer that has a “12” or “21” adjacent pair in its base‑9
   representation while k-1 does not.  A simple exhaustive search is
   sufficient for the required range. */

#include <stdbool.h>

/* Return true if x (written in base‑9) contains an adjacent pair "12" or "21". */
static bool has12(seq_t x)
{
    /* Any number with less than two digits cannot contain the pair. */
    if (x < (seq_t)9) return false;

    seq_t next = x % (seq_t)9;
    x /= (seq_t)9;
    while (x > (seq_t)0)
    {
        seq_t cur = x % (seq_t)9;
        if ((cur == (seq_t)1 && next == (seq_t)2) ||
            (cur == (seq_t)2 && next == (seq_t)1))
            return true;
        next = cur;
        x /= (seq_t)9;
    }
    return false;
}

/* a(n) returns the n‑th term of A043188, n ≥ 1. */
seq_t a(idx_t n)
{
    idx_t  count   = 0;       /* how many terms have been found */
    seq_t  k       = 1;       /* current integer to test */
    bool   prev_has = false;  /* has12(k‑1) for k==1 is false */

    while (count < n)
    {
        bool cur_has = has12(k);
        if (cur_has && !prev_has)
        {
            ++count;
            if (count == n)
                return k;
        }
        prev_has = cur_has;
        ++k;
    }

    /* This point is never reached because n ≥ 1. */
    return (seq_t)0;
}
