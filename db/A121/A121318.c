/*
 * A121318  Molecular topological indices of the path graphs P_n.
 * https://oeis.org/A121318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3-3*n^2+13*n-18)/3 for n≥2, with a(1)=0
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        return (seq_t)0;
    }
    seq_t n1 = (seq_t)n;
    /* numerator = 2*n1^3 - 3*n1^2 + 13*n1 - 18 */
    seq_t part1 = (seq_t)2 * n1 * n1 * n1;
    seq_t part2 = (seq_t)3 * n1 * n1;
    seq_t part3 = (seq_t)13 * n1;
    seq_t numerator = part1 - part2 + part3 - (seq_t)18;
    return numerator / (seq_t)3;
}
