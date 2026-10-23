/*
 * A182751  a(1)=1, a(2)=3, a(3)=6; a(n) = 3*a(n-2) for n > 3.
 * https://oeis.org/A182751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(1)=1, a(2)=3, a(3)=6; a(n)=3*a(n-2) for n>3
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq3(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        res *= ((seq_t)3);
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }
    if (n & 1) {            /* odd n >= 3 */
        idx_t k = (n - 1) / 2;
        seq_t p = pow_seq3(k - 1);
        return p * ((seq_t)6);
    } else {                /* even n */
        idx_t k = n / 2;
        return pow_seq3(k);
    }
}
