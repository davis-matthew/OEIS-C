/*
 * A198442  Number of sequences of n coin flips that win on the last flip, if
 *          the sequence of flips ends with (1,1,0) or (1,0,0).
 * https://oeis.org/A198442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 - 5 - 3*(-1)^n)/8
 * Range:     u8 n <= 11, u16 n <= 338, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn;        /* n^2 */
    num = num * 2;              /* 2*n^2 */
    num -= 5;                   /* 2*n^2 - 5 */

    /* add or subtract 3 depending on parity */
    if ((n & 1) == 0)
        num -= 3;               /* even n -> -3 */
    else
        num += 3;               /* odd n  -> +3 */

    return num / 8;             /* integer division */
}
