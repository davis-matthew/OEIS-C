/*
 * A112710  Partial sums of Catalan numbers A000108 multiplied by powers of
 *          -3.
 * https://oeis.org/A112710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=0..n} Catalan(k) * (-3)^k where Catalan(0)=1 and
 *            Catalan(k+1)=Catalan(k)*2*(2k+1)/(k+2). For k=0 return 1, for
 *            k>0 iterate
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 19, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the zero index immediately */
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    seq_t cat = 1;                /* Catalan(0) */
    seq_t pow = 1;                /* (-3)^0 */

    /* k = 0 contribution */
    sum = cat * pow;              /* 1*1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* update Catalan(k) from Catalan(k-1) */
        idx_t prev = k - 1;
        seq_t numer = (seq_t)(4 * prev + 2);   /* 2*(2*prev+1) = 4*prev+2 */
        seq_t denom = (seq_t)(k + 1);          /* n+2 where n=prev */
        cat = cat * numer / denom;            /* integer division */

        /* update (-3)^k */
        pow = pow * (seq_t)-3;

        /* accumulate */
        sum += cat * pow;
    }

    return sum;
}
