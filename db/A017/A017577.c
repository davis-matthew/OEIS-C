/*
 * A017577  a(n) = (12*n + 4)^9.
 * https://oeis.org/A017577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 4)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 11, u128 n <= 1592,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)12 * (seq_t)n + (seq_t)4;
    seq_t res  = 1;
    /* exponentiation by successive multiplication (9 times) */
    for (int i = 0; i < 9; ++i) {
        res *= base;
    }
    return res;
}
