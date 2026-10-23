/*
 * A106324  Greater of number pair whose squares are reversals of each other,
 *          with no leading zeros allowed.
 * https://oeis.org/A106324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   isqrt, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106324: greater of pair whose squares are reversals of each other (no leading zeros).  
   Algorithm: iterate over increasing smaller member a, compute a², reverse it, test if the
   result is a perfect square b²; when a < b, output b. */

#include "../utils/rev.h"
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t a_val = 1;
    for (;;) {
        /* a² */
        seq_t a_sq = (seq_t)a_val * (seq_t)a_val;

        /* skip if square ends with 0 (would lead to a leading zero after reversal) */
        if ((a_sq % (seq_t)10) == 0) {
            ++a_val;
            continue;
        }

        seq_t rev_sq = rev(a_sq);
        seq_t b = isqrt(rev_sq);
        if (b * b == rev_sq && b != (seq_t)0) {
            /* pair found */
            if ((seq_t)a_val < b) {
                ++count;
                if (count == n)
                    return b;          /* greater member */
            }
        }
        ++a_val;
    }
}
