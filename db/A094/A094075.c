/*
 * A094075  Denominator of I(n)=integral_{x=0 to 1/n}(x^2-1)^3 dx.
 * https://oeis.org/A094075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 105*n^7
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;      /* n */
    seq_t n2  = x * x;         /* n^2 */
    seq_t n3  = n2 * x;        /* n^3 */
    seq_t n6  = n3 * n3;       /* n^6 */
    seq_t n7  = n6 * x;        /* n^7 */
    return n7 * (seq_t)105;    /* 105 * n^7 */
}
