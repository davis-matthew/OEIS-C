/*
 * A198856  a(n) = 9*8^n - 1.
 * https://oeis.org/A198856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*8^n - 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = (seq_t)1;
    if (n != 0) {
        seq_t base = (seq_t)8;
        idx_t exp = n;
        while (exp > 0) {
            if (exp & (idx_t)1) {
                pow = pow * base;
            }
            exp >>= 1;
            if (exp) {
                base = base * base;
            }
        }
    }
    seq_t res = pow * (seq_t)9;
    return res - (seq_t)1;
}
