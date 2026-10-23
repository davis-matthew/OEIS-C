/*
 * A112522  Expansion of
 *          (1+3*x+14*x^2-10*x^3-10*x^4+16*x^5+15*x^6-15*x^7-2*x^8+4*x^9+8*x^10)
 *          / ((1+4*x^2)*(1-x^2+x^4)*(1+x^2)^2).
 * https://oeis.org/A112522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -5*a(n-2) - 4*a(n-4) - a(n-6) - 5*a(n-8) - 4*a(n-10) for
 *            n > 10 Initial values: a(0)=1, a(1)=3, a(2)=9, a(3)=-25,
 *            a(4)=-59, a(5)=129, a(6)=273, a(7)=-563, a(8)=-1145, a(9)=2313,
 *            a(10)=4651
 * Range:     i8 n <= 4, i16 n <= 12, i32 n <= 28, i64 n <= 60, i128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[11] =
    {1, 3, 9, -25, -59, 129, 273, -563, -1145, 2313, 4651};

    if (n <= 10u)
        return init[(idx_t)n];

    seq_t buf[11];
    for (idx_t i = 0; i <= 10; ++i)
        buf[i] = init[i];

    for (idx_t k = 11; k <= n; ++k)
    {
        seq_t val = 0;
        val -= 5 * buf[(k - 2) % 11];
        val -= 4 * buf[(k - 4) % 11];
        val -= buf[(k - 6) % 11];
        val -= 5 * buf[(k - 8) % 11];
        val -= 4 * buf[(k - 10) % 11];
        buf[k % 11] = val;
    }

    return buf[n % 11];
}
