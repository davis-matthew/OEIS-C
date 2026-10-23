/*
 * A044607  Numbers n such that string 4,7 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose octal representation
 *            contains the substring "47" but whose successor's octal
 *            representation does not contain "47"
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether the octal representation of x contains the substring "47".
   The function works for all integer types seq_t supported by the build. */
static bool has47(seq_t x)
{
    seq_t t = x;
    int prev = -1;          /* previous digit (less significant) */
    while (t > 0) {
        seq_t dseq = t % (seq_t)8;      /* current digit */
        int d = (int)dseq;              /* d < 8, safe cast */
        if (prev == 7 && d == 4) {
            return true;                /* found "47" (reversed order) */
        }
        prev = d;
        t /= (seq_t)8;
    }
    return false;
}

/* Return the n-th element of A044607 (1‑based). */
seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many terms found so far */
    seq_t cand  = 1;          /* current candidate to test */

    while (found < n) {
        if (has47(cand) && !has47(cand + (seq_t)1)) {
            found++;
            if (found == n) {
                return cand;
            }
        }
        ++cand;               /* next integer to test */
    }

    /* Should never reach here for n >= 1 */
    return (seq_t)0;
}
