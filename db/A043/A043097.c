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
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for the nth member of sequence A043097
   (numbers whose base‑3 representation contains an adjacent pair of
   digits 0 and 1, but whose predecessor does not). */

#include <stdbool.h>

static inline bool has_adj_pair(seq_t x) {
    seq_t last = (seq_t)3;          /* sentinel > any base‑3 digit */
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)3;
        if (last <= (seq_t)2) {
            if ((last == (seq_t)0 && d == (seq_t)1) ||
                (last == (seq_t)1 && d == (seq_t)0)) {
                return true;
            }
        }
        last = d;
        x = x / (seq_t)3;
    }
    return false;
}

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
