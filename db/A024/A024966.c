/*
 * A024966  7 times triangular numbers: 7*n*(n+1)/2.
 * https://oeis.org/A024966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n*(n+1)/2
 * Range:     u8 n <= 8, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the triangular number n*(n+1)/2 without intermediate overflow,
       then multiply by 7. */
    idx_t a_idx = n;
    idx_t b_idx = n + 1;

    /* Divide one of the factors by 2 to keep the intermediate product in range */
    if ((a_idx & 1u) == 0) {
        a_idx >>= 1;
    } else {
        b_idx >>= 1;
    }

    seq_t tri = (seq_t)a_idx * (seq_t)b_idx; /* n*(n+1)/2 */
    return tri * 7;                           /* 7 * triangular */
}
