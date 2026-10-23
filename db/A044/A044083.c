/*
 * A044083  Numbers k such that string 1,1 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_consecutive_ones_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A044083: numbers k such that the base‑5 representation of k
 * contains the substring “11” while the base‑5 representation of k‑1
 * does not.  The function searches for the nth such number. */
#include <stdbool.h>
#include "../utils/has_consecutive_ones_base5.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, but return 0 for safety */
        return (seq_t)0;

    idx_t count = 0;      /* how many valid numbers found so far */
    idx_t k = 1;          /* current candidate */

    for (;; ++k) {
        if (has_consecutive_ones_base5(k) && !has_consecutive_ones_base5(k - 1)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
