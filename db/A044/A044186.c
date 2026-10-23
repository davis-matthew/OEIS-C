/*
 * A044186  Numbers k such that string 6,6 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has66
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k such that the base‑7 representation of k contains
   the substring 66 but the representation of k‑1 does not (search‑based). */

#include <stdbool.h>
#include "../utils/has66.h"

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for index 0, return 0 */
        return (seq_t)0;

    idx_t count = 0;
    seq_t k = 1;             /* start searching from the first positive integer */

    while (count < n) {
        if (has66(k) && !has66(k - (seq_t)1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
    /* Should never reach here */
    return (seq_t)0;
}
