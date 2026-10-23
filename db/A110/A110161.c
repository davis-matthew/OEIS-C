/*
 * A110161  Expansion of x*(1-x^2)/(1-x^2+x^4).
 * https://oeis.org/A110161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) = a(n-2)-a(n-4), with a(0)=0, a(1)=1, a(2)=0, a(3)=0
 *            (periodic of length 12)
 * Range:     i8 n <= 8483, i16 n <= 8483, i32 n <= 8483, i64 n <= 8483,
 *            i128 n <= 8483, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 0;
    if (n == 3) return 0;

    seq_t buf[4] = {0, 1, 0, 0};   /* buf[0]=a(n-4), buf[1]=a(n-3), buf[2]=a(n-2), buf[3]=a(n-1) */
    seq_t ai = 0;

    for (idx_t i = 4; i <= n; ++i) {
        ai = buf[2] - buf[0];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = ai;
    }
    return ai;
}
