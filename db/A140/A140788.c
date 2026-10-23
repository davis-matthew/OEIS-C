/*
 * A140788  a(n) = 6*4^n + 2.
 * https://oeis.org/A140788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*4^n + 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = 1;          /* 4^n */
    seq_t base = 4;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) pow *= base;
        base *= base;
        exp >>= 1;
    }
    return 6 * pow + 2;
}
