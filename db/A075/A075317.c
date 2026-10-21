/*
 * A075317  Pair the odd numbers such that the k-th pair is (r, r+2k) where r
 *          is the smallest odd number not included earlier:
 *          (1,3),(5,9),(7,13),(11,19),(15,25),(17,29),(21,35),(23,39),(27,45),...
 *          This is the sequence of the first member of pairs.
 * https://oeis.org/A075317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor(n*phi) - 1 where phi = (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1, but safe */

    seq_t nn = (seq_t)n;
    seq_t m = nn * nn;            /* n^2  (seq_t math)               */
    m = m * ((seq_t)5);           /* 5*n^2   */
    seq_t s = floor_sqrt(m);      /* floor(n*sqrt(5))                */
    seq_t a_val = (nn + s) / ((seq_t)2); /* floor(n*phi)   */
    return ((seq_t)2 * a_val) - ((seq_t)1);
}
