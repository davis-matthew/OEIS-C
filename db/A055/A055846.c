/*
 * A055846  a(n) = 25*6^(n-2), with a(0)=1 and a(1)=4.
 * https://oeis.org/A055846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 25*6^(n-2), a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* exponentiation by squaring: compute 6^(n-2) */
    seq_t pow6 = 1;
    seq_t base = 6;
    idx_t exp = n - 2;
    while (exp != 0) {
        if (exp & 1) pow6 *= base;
        base *= base;
        exp >>= 1;
    }

    return (seq_t)25 * pow6;
}
