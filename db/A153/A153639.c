/*
 * A153639  Evilness of triangular numbers.
 * https://oeis.org/A153639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the triangular number n*(n+1)/2 is evil (has an even
 *            number of 1 bits in its binary representation), else 0
 * Range:     u8 n <= 22, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute the n-th triangular number */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    /* popcount parity: 0 for even, 1 for odd */
    int parity = 0;
    seq_t x = tn;
    while (x) {
        parity ^= (int)(x & 1);
        x >>= 1;
    }
    /* evilness is 1 when parity is even (parity == 0) */
    return (seq_t)(parity == 0);
}
