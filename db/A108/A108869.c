/*
 * A108869  E.g.f. : exp(6x)/(1-x).
 * https://oeis.org/A108869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*a(n-1) + 6^n, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;          /* a(0) */
    seq_t pow6   = 1;          /* 6^0 */

    for (idx_t i = 1; i <= n; ++i) {
        pow6 *= 6;                  /* now pow6 = 6^i */
        result = (seq_t)i * result + pow6;  /* a(i) = i*a(i-1) + 6^i */
    }
    return result;
}
