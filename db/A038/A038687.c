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
 * Validated: standard, pass, 109 terms through n = 972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by scanning successive integers and testing digit count. */
#include <stddef.h>
#include <stdbool.h>

/* Test whether x has at least two non‑zero decimal digits. */
static bool has_two_nonzero(seq_t x) {
    unsigned cnt = 0;
    while (x != (seq_t)0) {
        seq_t d = x % (seq_t)10;
        if (d != (seq_t)0) {
            ++cnt;
            if (cnt >= 2) return true;
        }
        x /= (seq_t)10;
    }
    return false;
}

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
