/*
 * A017075  a(n) = (8*n)^11.
 * https://oeis.org/A017075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 7, u128 n <= 397,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 8;
    seq_t res  = 1;
    idx_t  exp = 11;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}
