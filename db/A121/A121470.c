/*
 * A121470  Expansion of x*(1+5*x+2*x^2+x^3)/((1+x)*(1-x)^3).
 * https://oeis.org/A121470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n^2 - 12*n + 5 + 3*(-1)^n) / 8
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* convert index to seq_t */
    seq_t val = nn * nn;            /* n^2 */
    val = val * 18;                 /* 18*n^2 */
    val = val - nn * 12;            /* -12*n */
    val = val + 5;                  /* +5 */
    if ((n & 1) == 0)               /* even n -> add 3 */
        val = val + 3;
    else                            /* odd n  -> subtract 3 */
        val = val - 3;
    return val / 8;                 /* divide by 8 */
}
