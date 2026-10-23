/*
 * A017635  a(n) = (12*n + 9)^7.
 * https://oeis.org/A017635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 9)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 46, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)12 * n + (seq_t)9;
    seq_t base2  = base * base;          /* (12n+9)^2 */
    seq_t base4  = base2 * base2;        /* (12n+9)^4 */
    seq_t result = base4 * base2 * base; /* (12n+9)^7 */
    return result;
}
