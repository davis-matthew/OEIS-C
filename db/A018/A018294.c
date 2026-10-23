/*
 * A018294  Divisors of 124.
 * https://oeis.org/A018294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = sorted divisors of 124
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n <= 3) {
        return (seq_t)1 << ((idx_t)n - 1);
    }
    if (n <= 6) {
        return (seq_t)31 * ((seq_t)1 << ((idx_t)n - 4));
    }
    return 0;
}
