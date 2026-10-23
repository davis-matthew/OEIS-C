/*
 * A128422  Projective plane crossing number of K_{4,n}.
 * https://oeis.org/A128422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)*(n-2)/3)
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) {
        return 0;
    }
    seq_t n1 = (seq_t)(n - 1);
    seq_t n2 = (seq_t)(n - 2);
    return (n1 * n2) / 3;
}
