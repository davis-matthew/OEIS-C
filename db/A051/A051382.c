/*
 * A051382  Numbers k whose base 3 expansion matches (0|1)*(02)?(0|1)* (no
 *          more than one "02" allowed in midst of 0's and 1's).
 * https://oeis.org/A051382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 3602, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 82 terms through n = 8856 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (search) a(n) = the nth non‑negative integer k such that the base‑3
   representation of 2k contains only digits 0 or 2, with at most one
   pair of adjacent 1's.  The offset is 1, so a(1)=0. */
#include <stdbool.h>

/* Return true iff 2*k meets the base‑3 pattern described above. */
static bool valid_base3_pairs(seq_t k)
{
    seq_t v = 2 * k;          /* transpose to 2k */
    int state = 0;            /* 0 – no pending 1, 1 – seen first 1 */
    bool had_pair = false;    /* true if a pair of 1's has already appeared */

    while (v > 0)
    {
        int d = (int)(v % 3); /* current least‑significant digit */
        v /= 3;

        if (d == 1)
        {
            if (state == 0)            /* first 1 of a potential pair */
                state = 1;
            else if (state == 1)       /* second 1 of the pair */
            {
                if (had_pair)            /* more than one pair */
                    return false;
                had_pair = true;
                state = 0;               /* pair closed – no more 1's allowed */
            }
            else
                return false;           /* should never happen */
        }
        else                        /* digit is 0 or 2 */
        {
            if (state == 1)            /* expected a second 1 */
                return false;
        }
    }

    /* If we finish while still waiting for the second 1, the pattern is invalid. */
    return state == 0;
}

/* OEIS sequence A051382: kth term in the list of integers whose
   base‑3 representation of 2k satisfies the 0/2 only with at most
   one adjacent pair of 1's. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;

    while (1)
    {
        if (valid_base3_pairs(k))
        {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
