/*
 * A106510  Expansion of (1+x)^2/(1+x+x^2).
 * https://oeis.org/A106510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(0)=1, a(1)=1, a(2)=-1, a(3)=0, a(n)=a(n-3) for n>=4
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic of period 3 for n>=3 with values {0,1,-1}. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)(-1);
    idx_t mod = (n - (idx_t)3) % (idx_t)3;
    if (mod == (idx_t)0) return (seq_t)0;
    if (mod == (idx_t)1) return (seq_t)1;
    return (seq_t)(-1);
}
