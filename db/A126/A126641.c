/*
 * A126641  a(n) is the number of integers k less than 10^n such that the
 *          decimal representation of k lacks the digit 1, at least one of
 *          digits 2,3, at least one of digits 4,5, at least one of digits 6,7
 *          and at least one of digits 8,9.
 * https://oeis.org/A126641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 16*5^n - 32*4^n + 24*3^n - 8*2^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_small(unsigned base, idx_t exp) {
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p5 = pow_seq_small(5, n);
    seq_t p4 = pow_seq_small(4, n);
    seq_t p3 = pow_seq_small(3, n);
    seq_t p2 = pow_seq_small(2, n);

    seq_t res = 16 * p5;
    res -= 32 * p4;
    res += 24 * p3;
    res -= 8 * p2;
    res += 1;

    return res;
}
