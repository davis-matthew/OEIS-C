/*
 * A047237  Numbers that are congruent to {0, 1, 2, 4} mod 6.
 * https://oeis.org/A047237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 * floor((n-1)/4) + {0,1,2,4}[(n-1) mod 4]
 * Range:     u8 n <= 171, u16 n <= 7845, u32 n <= 7845, u64 n <= 7845,
 *            u128 n <= 7845, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is >=1 by the problem statement */
    idx_t k = n - 1;          /* 0‑based index */
    idx_t q = k / 4;          /* number of complete 6‑blocks before the term */
    idx_t m = k % 4;          /* offset inside the current block (0‑3) */
    static const seq_t vals[4] = {0, 1, 2, 4};
    return (seq_t)(6 * q) + vals[m];
}
