/*
 * A083227  a(n) = (9*10^n + (-10)^n)/10.
 * https://oeis.org/A083227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9*10^n + (-10)^n)/10 = 10^n if n is even, and =
 *            8*10^(n-1) if n is odd, with a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow10(idx_t exp) {
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= (seq_t)10;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if ((n & 1) == 0) {                 /* n even */
        return pow10(n);
    } else {                               /* n odd */
        seq_t ten_pow = pow10(n - 1);
        ten_pow *= (seq_t)8;
        return ten_pow;
    }
}
