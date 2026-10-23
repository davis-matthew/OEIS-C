/*
 * A044200  Numbers n such that string 1,5 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044200: numbers n such that the base‑8 representation of n
   contains the substring "15" while n‑1 does not.  This function scans
   through the integers in increasing order, testing each candidate
   until the n‑th such number is found.  It works for all supported
   seq_t types. */

#include <stdbool.h>

/* Return true if the octal representation of x contains the digit pair
   1 followed by 5 (most‑to‑least significant). */
static bool has15(seq_t x)
{
    while (x >= 8) {
        seq_t low  = x % 8;
        seq_t high = (x / 8) % 8;
        if (high == 1 && low == 5)
            return true;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset < 1 */

    idx_t count = 0;
    seq_t cand  = (seq_t)1;

    while (1) {
        if (has15(cand) && !has15(cand - 1)) {
            ++count;
            if (count == n)
                return cand;
        }
        cand++;
    }
}
