/*
 * A044347  Numbers n such that string 1,5 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search starting from 15 and checking each
   integer whether it contains the decimal substring "15" while the
   predecessor does not. */
#include <stdbool.h>

static inline bool has15(seq_t x)
{
    /* Does x contain the two–digit pattern 15? */
    for (seq_t t = x; t >= 15; t /= 10) {
        if (t % 100 == 15) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Return the n‑th number for which "15" occurs in the
       decimal representation of the number but not in the
       representation of n-1. */
    idx_t counted = 0;
    seq_t x = 15;              /* first number that could satisfy the condition */

    for (;; ++x) {
        if (has15(x) && !has15(x - 1)) {
            ++counted;
            if (counted == n) return x;
        }
    }
}
