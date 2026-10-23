/*
 * A158622  Numerator of the reduced fraction A158620(n)/A158621(n).
 * https://oeis.org/A158622
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = numerator of the reduced fraction 2*(n^2+n+1) /
 *            (3*n*(n+1))
 * Range:     u8 n <= 8, u16 n <= 182, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* below offset – undefined */

    seq_t nn   = (seq_t)n;
    seq_t t    = nn * nn + nn + 1;    /* n^2 + n + 1 */
    seq_t num  = 2 * t;               /* 2(n^2+n+1) */
    seq_t den  = 3 * nn * (nn + 1);   /* 3*n*(n+1) */

    seq_t g = gcd_seq(num, den);      /* gcd of numerator and denominator */
    return num / g;                   /* reduced numerator */
}
