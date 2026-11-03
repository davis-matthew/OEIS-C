/*
 * A043119  Numbers k such that 0 and 4 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k whose base‑6 representation * contains
 *            the digits 0 and 4 adjacent (in either order) but k−1 does *
 *            not. Computed by a straightforward linear scan
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_adjacent_0_and_4(seq_t x)
{
    if (x == (seq_t)0) return false;               /* only digit 0, no adjacency */

    seq_t tmp = x;
    int prev = -1;                                 /* previous base‑6 digit */

    while (tmp > (seq_t)0) {
        int digit = (int)(tmp % (seq_t)6);          /* current least‑significant digit */

        if (prev >= 0 &&
            ((prev == 0 && digit == 4) || (prev == 4 && digit == 0)))
            return true;                           /* adjacency found */

        prev = digit;                              /* set for next iteration */
        tmp /= (seq_t)6;                           /* shift right one base‑6 digit */
    }
    return false;                                  /* no adjacency detected */
}

seq_t a(idx_t n)
{
    idx_t count = 0;                               /* number of terms found so far */
    seq_t k = 0;                                   /* current integer being examined */

    while (count < n) {
        k++;                                       /* examine next integer */

        if (has_adjacent_0_and_4(k) &&
            !has_adjacent_0_and_4(k - (seq_t)1)) {  /* k has adjacency, k‑1 does not */
            count++;                               /* accept this term */

            if (count == n)                       /* wanted k reached */
                return k;
        }
    }

    return (seq_t)0;                               /* unreachable, defensive return */
}
