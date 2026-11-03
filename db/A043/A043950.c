/*
 * A043950  Numbers k such that 3 and 4 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043950: Numbers k such that 3 and 4 occur juxtaposed in the base‑8
   representation of k but not of k+1.  
   Implementation performs a linear enumeration until the n‑th qualifying
   value is found.  The helper function detects either "34" or "43" as
   consecutive digits in octal. */
#include <stdbool.h>

/* Return true if x contains the pattern 3 and 4 adjacent in either
   order in base‑8. */
static bool has34_oct(seq_t x)
{
    int prev = -1;                     /* digit processed most recently   */
    while (x != (seq_t)0) {
        int cur = (int)(x % (seq_t)8);  /* least‑significant base‑8 digit */
        if ((cur == 3 && prev == 4) || (cur == 4 && prev == 3))
            return true;
        prev = cur;
        x = x / (seq_t)8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* offset starts at 1; undefined for n=0 */
        return (seq_t)0;

    idx_t found = 0;          /* number of qualifying k found so far */
    seq_t k = 0;              /* candidate k, will start from 1 */

    while (found < n) {
        ++k;
        if (has34_oct(k)) {
            if (!has34_oct(k + (seq_t)1)) {
                ++found;
                if (found == n)
                    return k;
            }
        }
    }

    /* This point should never be reached for valid inputs */
    return (seq_t)0;
}
