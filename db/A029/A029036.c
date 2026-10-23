/*
 * A029036  Expansion of 1/((1-x)*(1-x^3)*(1-x^4)*(1-x^10)).
 * https://oeis.org/A029036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 54*n^2 + 423*n + 9*n*(-1)^n + 1600)/1440)
 * Range:     u8 none, u16 n <= 23, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                          /* convert index to seq_t */
    seq_t term = 0;

    term += (seq_t)2 * sn * sn * sn;              /* 2*n^3 */
    term += (seq_t)54 * sn * sn;                 /* 54*n^2 */
    term += (seq_t)423 * sn;                     /* 423*n */

    seq_t n9 = (seq_t)9 * sn;                     /* 9*n */

    if (n & 1)                                   /* n odd: subtract 9*n */
        term -= n9;
    else                                         /* n even: add 9*n */
        term += n9;

    term += (seq_t)1600;                         /* +1600 */

    return term / (seq_t)1440;                   /* floor division by 1440 */
}
