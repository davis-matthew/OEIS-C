/*
 * A005796  Degrees of fundamental invariants of Weyl group W(E_8).
 * https://oeis.org/A005796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is one plus the n-th positive integer coprime to 30
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t offsets[] = {1, 7, 11, 13, 17, 19, 23, 29};
    
    idx_t period = (n - 1) / 8;
    idx_t position = (n - 1) % 8;
    
    seq_t k = (seq_t)period * 30 + offsets[position];
    return k + 1;
}
