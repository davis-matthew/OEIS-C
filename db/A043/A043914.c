/*
 * A043914  Numbers k such that 0 and 4 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function implements the OEIS definition:
 * a(n) = the n‑th integer k such that the base‑7 representation of k
 * contains the digits 0 and 4 adjacent (in any order) and the base‑7
 * representation of k+1 does not contain the digits 0 and 4 adjacent.
 * The implementation walks through the integers sequentially, checking
 * the adjacency property by successive division by 7.
 */
#include <stdbool.h>

static bool has_adjacent_zero_and_four(seq_t x)
/* Return true if the base‑7 representation of x contains 0 and 4
   as adjacent digits (either "04" or "40"). */
{
    if (x < (seq_t)7)   /* less than two digits, cannot have adjacency */
        return false;

    seq_t prev = x % (seq_t)7;
    x /= (seq_t)7;
    while (x > 0) {
        seq_t cur = x % (seq_t)7;
        if ((cur == (seq_t)0 && prev == (seq_t)4) ||
            (cur == (seq_t)4 && prev == (seq_t)0))
            return true;
        prev = cur;
        x /= (seq_t)7;
    }
    return false;
}

seq_t a(idx_t n)
/* Return a(n) for the OEIS sequence A043914. */
{
    idx_t count = 0;
    seq_t k = 0;

    while (1) {
        ++k;
        if (has_adjacent_zero_and_four(k)) {
            seq_t kp1 = k + (seq_t)1;
            if (!has_adjacent_zero_and_four(kp1)) {
                ++count;
                if (count == n)
                    return k;
            }
        }
    }
}
