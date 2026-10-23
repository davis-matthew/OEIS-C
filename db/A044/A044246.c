/*
 * A044246  Numbers k such that string 7,3 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_73
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044246: a(n) = the n-th integer k such that the octal
 * representation of k contains the substring 73, but the octal
 * representation of (k‑1) does NOT contain that substring. */
#include <stdbool.h>
#include "../utils/has_73.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    while (1) {
        if (has_73(k) && !has_73(k - 1)) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
