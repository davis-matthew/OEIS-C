/*
 * A002783  a(n) = 2*(3^n - 2^n) + 1.
 * https://oeis.org/A002783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(3^n - 2^n) + 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t ipow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t pow3 = ipow((seq_t)3, n);
    seq_t pow2 = ipow((seq_t)2, n);
    return 2 * (pow3 - pow2) + 1;
}
