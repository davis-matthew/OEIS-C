/*
 * A074574  a(n) = 5^n + 7^n + 8^n.
 * https://oeis.org/A074574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n + 7^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p5 = pow_seq((seq_t)5, n);
    seq_t p7 = pow_seq((seq_t)7, n);
    seq_t p8 = pow_seq((seq_t)8, n);
    return p5 + p7 + p8;
}
