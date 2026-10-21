/*
 * A089011  a(n) = 1 if n is an exponent of the Weyl group W(E_7), 0
 *          otherwise.
 * https://oeis.org/A089011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n ∈ {1,5,7,9,11,13,17}, 0 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    switch (n) {
        case 1: case 5: case 7: case 9:
        case 11: case 13: case 17:
            return (seq_t)1;
        default:
            return (seq_t)0;
    }
}
