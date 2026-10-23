/*
 * A038687  Concatenate i >= 1 and j >= 1, then sort.
 * https://oeis.org/A038687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer with at least two non‑zero
 *            decimal digits
 * Range:     u8 n <= 235, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_two_nonzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by scanning successive integers and testing digit count. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_two_nonzero.h"

/* OEIS A038687: numbers with at least two non‑zero decimal digits. */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;   /* offset is 1; undefined for n==0 */

    seq_t cand = (seq_t)10;               /* start just below the first candidate */
    idx_t found = 0;

    while (1) {
        ++cand;
        if (has_two_nonzero(cand)) {
            ++found;
            if (found == n) return cand;
        }
    }
}
