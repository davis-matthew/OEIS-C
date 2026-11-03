/*
 * A039460  Numbers whose base-11 representation has the same nonzero number
 *          of 2's and 6's.
 * https://oeis.org/A039460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 966 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A039460: numbers whose base‑11 representation contains
   equal non‑zero counts of digits 2 and 6.  The function searches
   integers in order and returns the n‑th qualifying number.
*/

#include <stdbool.h>

/* Return true if v has the same positive number of 2’s and 6’s in base‑11. */
static bool has_equal_2_6(seq_t v)
{
    int c2 = 0, c6 = 0;
    while (v > 0) {
        int d = (int)(v % 11);
        if (d == 2)   ++c2;
        else if (d == 6) ++c6;
        v /= 11;
    }
    return (c2 > 0 && c2 == c6);
}

/* Sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined offset – safe fallback */

    seq_t v = 0;
    idx_t count = 0;

    /* Search until the n‑th qualifying number is found */
    while (1) {
        if (has_equal_2_6(v)) {
            ++count;
            if (count == n) return v;
        }
        ++v;  /* next integer to test */
    }
}
