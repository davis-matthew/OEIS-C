/*
 * A212346  Sequence of coefficients of x^0 in marked mesh pattern generating
 *          function Q_{n,132}^(0,4,0,0)(x).
 * https://oeis.org/A212346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=1, a(2)=2; for n>=3: a(n) = (n+3)*(n^2-4)/6
 *            (integral division, result is always an integer)
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)1;
    if (n == 2ULL) return (seq_t)2;

    seq_t nn = (seq_t)n;
    seq_t num = (nn + (seq_t)3) * ((nn * nn) - (seq_t)4);
    return num / (seq_t)6;
}
