/*
 * A009981  Powers of 37.
 * https://oeis.org/A009981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 37^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow = 1;
    seq_t base = 37;
    idx_t e = n;
    while (e > 0) {
        if (e & 1) pow *= base;
        base *= base;
        e >>= 1;
    }
    return pow;
}
