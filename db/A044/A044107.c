/*
 * A044107  Numbers k such that string 0,5 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 2034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_05_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044107: Numbers k such that base‑6 representation of k
   contains the adjacent digits 0,5 (0 more significant than 5) but
   base‑6 representation of k‑1 does not.  The function returns the nth
   such integer.  It searches integers in order, testing each for the
   required pattern. */

#include <stdbool.h>
#include "../utils/has_05_uint64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    while (count < n) {
        ++k;
        if (has_05_uint64((uint64_t)k) && !has_05_uint64((uint64_t)(k - 1))) {
            ++count;
            if (count == n)
                return (seq_t)k;   /* convert to seq_t as required */
        }
    }
    return (seq_t)0;  /* should never happen for valid n >= 1 */
}
