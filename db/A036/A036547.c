/*
 * A036547  a(n) = T(6,n), array T given by A048471.
 * https://oeis.org/A036547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*3^n - 31; a(0) = 1; a(n) = 3*a(n-1) + 62
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow3 = 1;
    seq_t base = 3;
    idx_t exp = n;
    while (exp != 0) {
        if (exp & 1) {
            pow3 *= base;
        }
        exp >>= 1;
        if (exp) {
            base *= base;
        }
    }
    pow3 *= 32;
    pow3 -= 31;
    return pow3;
}
