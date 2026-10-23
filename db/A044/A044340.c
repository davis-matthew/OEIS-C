/*
 * A044340  Numbers n such that string 0,8 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044340: numbers n that contain the substring "08" in
   decimal representation, while n‑1 does not.  The function scans
   integers sequentially until the n‑th such number is found.  This
   routine works for any seq_t defined in the build (fixed width or
   arbitrary‑precision), using only integer arithmetic and the
   operations provided by seq_t. */
#include <stdbool.h>

/* Return true if x contains the decimal substring 08. */
static bool has08(seq_t x)
{
    if (x == 0)
        return false;          /* “0” does not contain “08” */

    int prev = -1;
    while (x > 0) {
        int digit = (int)(x % 10);
        if (digit == 0 && prev == 8)   /* reversed digits detect 08 */
            return true;
        prev = digit;
        x = x / 10;
    }
    return false;
}

/* Return the n‑th term of A044340 (1‑based index). */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand = 1;

    while (1) {
        if (has08(cand) && !has08(cand - 1)) {
            found++;
            if (found == n)
                return cand;
        }
        cand++;               /* cand is unsigned, wraps only if it exceeds seq_t limits */
    }
}
