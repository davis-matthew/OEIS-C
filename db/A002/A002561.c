/*
 * A002561  a(n) = n^5 + 1.
 * https://oeis.org/A002561
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + 1
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t abs_n = (n < 0) ? -n : n;
    seq_t x = (seq_t)abs_n;
    seq_t x2 = x * x;
    seq_t x4 = x2 * x2;
    seq_t x5 = x4 * x;
    
    if (n < 0) {
        return 1 - x5;
    } else {
        return x5 + 1;
    }
}
