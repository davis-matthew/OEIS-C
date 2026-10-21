/*
 * A074570  a(n) = 4^n + 8^n + 9^n.
 * https://oeis.org/A074570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 8^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t term4 = pow_seq((seq_t)4, n);
    seq_t term8 = pow_seq((seq_t)8, n);
    seq_t term9 = pow_seq((seq_t)9, n);
    return term4 + term8 + term9;
}
