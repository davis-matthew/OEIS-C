/*
 * A071191  Least m>n such that the number of prime factors of m and n differ
 *          at most by 1.
 * https://oeis.org/A071191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8739 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A071191: least m>n such that |Ω(m)-Ω(n)| <= 1 */
#include <stdint.h>
#include "../utils/bigomega_seq.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1) {
        return (seq_t)0;
    }
    seq_t w = bigomega_seq((seq_t)n);
    idx_t m = n + (idx_t)1;

    for (;; ++m) {
        seq_t wm   = bigomega_seq((seq_t)m);
        seq_t diff = wm > w ? wm - w : w - wm;
        if (diff <= (seq_t)1) {
            return (seq_t)m;
        }
    }
}
