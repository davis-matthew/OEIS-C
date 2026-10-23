/*
 * A044331  Numbers k such that the string 8,8 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 792, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_consecutive_88_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A044331:
   Numbers k such that the string 8,8 occurs in the base‑9 representation of k
   but does not occur in the base‑9 representation of k−1.  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_consecutive_88_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* n >= 1 for OEIS indexing, but guard anyway */

    idx_t count = 0;
    idx_t k = 1;
    bool prev_has = false;   /* k-1 = 0 has no '88' */

    while (count < n) {
        bool cur_has = has_consecutive_88_idx(k);
        if (cur_has && !prev_has) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        prev_has = cur_has;
        ++k;
    }

    return 0;   /* unreachable */
}
