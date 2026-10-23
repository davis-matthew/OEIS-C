/*
 * A008679  Expansion of 1/((1-x^3)*(1-x^4)).
 * https://oeis.org/A008679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n/3) + floor(-n/4)
 * Range:     u8 n <= 260, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                       /* constant part          */
    res += (seq_t)n / 3;                 /* floor(n/3)               */
    /* floor(-n/4) = -(n/4 + (n%4 != 0))    */
    idx_t q = n / 4;                     /* n/4                      */
    if (n & 3)                          /* n%4 != 0 ? add 1          */
        q++;
    res -= (seq_t)q;                     /* subtract floor(-n/4)   */
    return res;
}
