/*
 * A136264  Expansion of g.f. (1+x)^2*(x^2-6*x+1)/(x-1)^4.
 * https://oeis.org/A136264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; a(n)=8*n*(1 - n*n)/3 for n>0
 * Range:     i8 n <= 2, i16 n <= 16, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t n_seq = (seq_t)n;
    seq_t nsq   = n_seq * n_seq;          /* n^2   */
    seq_t temp  = (seq_t)1 - nsq;         /* 1 - n^2 */
    seq_t num   = (seq_t)8 * n_seq * temp;/* 8*n*(1-n^2) */
    return num / (seq_t)3;                /* division exact */
}
