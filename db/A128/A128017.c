/*
 * A128017  Expansion of (1+2x+x^2-x^3)/(1-x^2+x^4).
 * https://oeis.org/A128017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=2, a(3)=1;
   for n>=4: a(n)=a(n-2)-a(n-4)    */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 2;
    if (n == 3) return 1;

    seq_t buf[4] = {1, 2, 2, 1};
    for (idx_t i = 4; i <= n; ++i)
        buf[i & 3] = buf[(i - 2) & 3] - buf[(i - 4) & 3];
    return buf[n & 3];
}
