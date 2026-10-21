/*
 * A212310  a(n) = n! mod 4^n.
 * https://oeis.org/A212310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! mod 4^n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;

    const unsigned int bits = sizeof(seq_t) * 8;
    seq_t mask = 0;
    if (bits <= 64) {
        if (2 * n < bits) {
            mask = ((seq_t)1 << (2 * n)) - 1;
        }
    } else {
        mask = ((seq_t)1 << (2 * n)) - 1;
    }

    seq_t f = 1;
    for (idx_t i = 1; i <= n; ++i) {
        f *= (seq_t)i;
        if (mask) f &= mask;
    }
    return f;
}
