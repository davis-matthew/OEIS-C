/*
 * A039531  Numbers whose base-12 representation has the same nonzero number
 *          of 4's and 5's.
 * https://oeis.org/A039531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n), the nth integer whose base‑12 representation contains
   the same nonzero number of digits 4 and 5. */

#include <stdbool.h>

/* Return true if the base‑12 representation of x contains an equal
   and nonzero number of digits 4 and 5. */
static bool has_balanced_4_5(seq_t x)
{
    int cnt4 = 0;
    int cnt5 = 0;
    while (x != (seq_t)0) {
        seq_t rem = x % (seq_t)12;
        if (rem == (seq_t)4) {
            cnt4++;
        } else if (rem == (seq_t)5) {
            cnt5++;
        }
        x = x / (seq_t)12;
    }
    return (cnt4 == cnt5) && (cnt4 > 0);
}

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t num = (seq_t)1;
    while (found < n) {
        if (has_balanced_4_5(num)) {
            found++;
            if (found == n) {
                return num;
            }
        }
        num = num + (seq_t)1;
    }
    /* Should never reach here. */
    return (seq_t)0;
}
