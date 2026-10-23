/*
 * A126635  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks the digits 1,2,3, at
 *          least one of digits 4,5 and at least one of digits 6,7,8,9.
 * https://oeis.org/A126635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*5^n - 16*4^n + 14*3^n - 6*2^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 1) {
        return 0;
    }
    seq_t pow5 = power((seq_t)5, n);
    seq_t pow4 = power((seq_t)4, n);
    seq_t pow3 = power((seq_t)3, n);
    seq_t pow2 = power((seq_t)2, n);
    seq_t result = 8 * pow5 - 16 * pow4 + 14 * pow3 - 6 * pow2 + 1;
    return result;
}
