/*
 * A199417  a(n) = (3*7^n+1)/2.
 * https://oeis.org/A199417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*7^n + 1)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow  = (seq_t)1;   /* 7^0 */
    seq_t base = (seq_t)7;   /* 7 */
    idx_t e = n;

    while (e) {
        if (e & 1) {
            pow = pow * base;
        }
        base = base * base;
        e >>= 1;
    }

    seq_t res = pow * (seq_t)3 + (seq_t)1;
    res = res / (seq_t)2;
    return res;
}
