/*
 * A151556  Values of (n^5+47*n)/48 as n ranges over the numbers that are ==
 *          +-1 mod 6.
 * https://oeis.org/A151556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^5 + 47*n)/48) where n is the nth integer congruent to
 *            ±1 modulo 6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n-th term of the subsequence 1,5,7,11,13,17,... */
    idx_t m   = (n - 1) / 2;            /* floor((n-1)/2)  */
    idx_t val = (n & 1) ? 6 * m + 1 : 6 * m + 5;  /* element n of the subsequence */

    seq_t nn   = (seq_t)val;            /* cast to seq_t for arithmetic */
    seq_t num  = nn * nn * nn * nn * nn;/* n^5 */
    num       += (seq_t)47 * nn;        /* +47*n */
    return num / (seq_t)48;             /* divide by 48 */
}
