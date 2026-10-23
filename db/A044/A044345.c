/*
 * A044345  Numbers n such that string 1,3 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044345
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

/* Sequence: numbers n such that "13" occurs in the decimal
   representation of n but not of n–1.  No pre‑computed table
   is used; the terms are generated on the fly. */

#include <stdbool.h>

/* helper: true iff x contains the substring "13" */
static bool has13(idx_t x)
{
    bool prev3 = false;          /* true if the previous digit was 3 */
    while (x > 0) {
        int d = x % 10;          /* current least‑significant digit */
        if (prev3 && d == 1)     /* ...31.. pattern found */
            return true;
        prev3 = (d == 3);
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n ≥ 1, 1‑based indexing, behavior for n=0 is irrelevant */
    idx_t found = 0;
    for (idx_t x = 1; ; ++x) {          /* iterate positive integers */
        if (has13(x) && !has13(x - 1)) {
            ++found;
            if (found == n)
                return (seq_t)x;      /* cast to the target type */
        }
    }
}
