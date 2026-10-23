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
 * Helpers:   has59
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth positive integer that contains the decimal
 * substring "59" but the preceding integer (n-1) does not.  This is
 * computed by a linear search.
 */

#include <stdbool.h>
#include "../utils/has59.h"

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
