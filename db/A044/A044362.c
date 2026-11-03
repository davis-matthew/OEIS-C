/*
 * A044362  Numbers n such that string 3,0 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044362 – numbers n such that the string "30" occurs
   in the decimal representation of n but not in n-1.
   Computation is done by linear search; no hard‑coded table. */

#include <stdbool.h>

/* Helper: does x contain the adjacent digits 3 followed by 0 in base 10? */
static bool has30(seq_t x)
{
    if (x < (seq_t)10)          /* single‑digit numbers cannot contain two digits */
        return false;

    seq_t prev = x % (seq_t)10; /* lower digit of the current pair */
    x /= (seq_t)10;

    while (x != (seq_t)0) {
        seq_t curr = x % (seq_t)10;   /* higher digit of the pair */
        if (curr == (seq_t)3 && prev == (seq_t)0)
            return true;
        prev = curr;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Starting from 1, search forward for n numbers that satisfy the
       condition.  The search is independent for each call, which
       satisfies requirement 6 (any call order). */
    seq_t cur = (seq_t)0;
    idx_t found = 0;

    while (found < n) {
        cur++; /* advance to next integer */

        if (has30(cur) && !has30(cur - (seq_t)1))
            ++found;
    }
    return cur;
}
