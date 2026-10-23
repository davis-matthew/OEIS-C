/*
 * A028291  Expansion of 1/((1-x)^2(1-x^2)(1-x^3)(1-x^5)) in powers of x.
 * https://oeis.org/A028291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((n+1)*(n^3+23*n^2+173*n+451)/720)
 * Range:     u8 none, u16 n <= 10, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the idx_t index to seq_t to avoid overflow during
       intermediate arithmetic.  All operations below are exact integer
       arithmetic, and the +360 ensures rounding for the division by 720. */
    seq_t nn   = (seq_t)n;          /* n */
    seq_t n1   = nn + (seq_t)1;     /* n+1 */
    seq_t n2   = nn * nn;           /* n^2 */
    seq_t n3   = n2 * nn;           /* n^3 */
    seq_t poly = n3 + (seq_t)23*n2 + (seq_t)173*nn + (seq_t)451;
    seq_t num  = n1 * poly + (seq_t)360;   /* +0.5*720 for rounding */
    return num / (seq_t)720;
}
