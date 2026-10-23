/*
 * A100855  a(n) = n*(n^3-n^2+n+1)/2.
 * https://oeis.org/A100855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3-n^2+n+1)/2
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tn  = (seq_t)n;
    seq_t n2  = tn * tn;
    seq_t n3  = n2 * tn;
    seq_t term = n3 - n2 + tn + 1;
    if ((tn & 1) == 0) {        /* n is even, divide tn first */
        return (tn / 2) * term;
    } else {                    /* term is even for odd n */
        return tn * (term / 2);
    }
}
