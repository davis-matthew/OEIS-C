/*
 * A199491  a(n) = (11*7^n + 1)/2.
 * https://oeis.org/A199491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*7^n + 1)/2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 7^n by exponentiation by squaring */
    seq_t pow7 = (seq_t)1;
    seq_t base   = (seq_t)7;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) pow7 *= base;
        base *= base;
        exp >>= 1;
    }

    /* a(n) = (11*7^n + 1)/2, division is exact */
    seq_t result = ((seq_t)11 * pow7 + 1) / 2;
    return result;
}
