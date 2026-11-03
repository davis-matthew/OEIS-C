/*
 * A044391  Numbers n such that string 5,9 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth positive integer that contains the decimal
 * substring "59" but the preceding integer (n-1) does not.  This is
 * computed by a linear search.
 */

#include <stdbool.h>

/* Helper: returns true if x contains the substring "59" */
static bool has59(seq_t x)
{
    if (x == 0) return false;
    int prev = -1;                /* digit to the left (more significant) */
    while (x > 0) {
        int d = (int)(x % 10);
        if (d == 5 && prev == 9) return true;
        prev = d;
        x /= 10;
    }
    return false;
}

/* Main function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe guard */

    /* The first term is known */
    if (n == 1) return (seq_t)59;

    seq_t cand = (seq_t)59;
    idx_t  count = 1;    /* we've already accounted for the first term */

    while (count < n) {
        cand++;                     /* move to next integer */
        if (has59(cand) && !has59(cand - 1)) {
            count++;
        }
    }
    return cand;
}
