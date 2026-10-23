/*
 * A100413  Numbers k such that k is reversal(k)-th even composite number (k
 *          is A004086(k)-th even composite number).
 * https://oeis.org/A100413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 6*10^n - 8
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 10^n */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow10 *= (seq_t)10;
    }
    /* 6*10^n - 8 */
    pow10 *= (seq_t)6;
    pow10 -= (seq_t)8;
    return pow10;
}
