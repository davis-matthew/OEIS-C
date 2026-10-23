/*
 * A043097  Numbers k such that 0 and 1 occur juxtaposed in the base-3
 *          representation of k but not of k-1.
 * https://oeis.org/A043097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   has_adj_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for the nth member of sequence A043097
   (numbers whose base‑3 representation contains an adjacent pair of
   digits 0 and 1, but whose predecessor does not). */

#include <stdbool.h>
#include "../utils/has_adj_pair.h"

seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;        /* undefined for n<1 */

    idx_t count = (idx_t)0;
    seq_t k = (seq_t)1;

    while (count < n) {
        if (has_adj_pair(k)) {
            seq_t km = k - (seq_t)1;
            if (!has_adj_pair(km)) {
                ++count;
                if (count == n) return k;
            }
        }
        ++k;
    }
    return (seq_t)0;    /* unreachable */
}
