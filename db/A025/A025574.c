/*
 * A025574  T(2n,n+1), where T is the array defined in A025564.
 * https://oeis.org/A025574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of y^(n+1) in (y^2+2*y+1)*(y^2+y+1)^(2*n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* coefficient of y^j in (y^2 + y + 1)^(2*n-2) */
static seq_t coeff_y(idx_t n, idx_t j)
{
    /* exponent of the polynomial */
    idx_t m = 2*n - 2;

    /* j cannot be outside [0, 2*m] */
    if (j > 2*m) return 0;

    /* a ranges such that
       a>=max(0, j-m)   and   a<=j/2   and   b=j-2a satisfies 0<=b<=m-a          */
    idx_t a_start = j > m ? j - m : 0;
    idx_t a_end   = j/2;

    if (a_start > a_end) return 0;

    seq_t sum = 0;
    for (idx_t a = a_start; a <= a_end; ++a)
    {
        idx_t b = j - 2*a;           /* exponent contributed by y^1 terms   */
        if (b > m - a) continue;     /* safeguard, but already covered   */

        /* multinomial coefficient: C(m, a) * C(m-a, b)  */
        seq_t term = binom(m, a);
        term = term * binom(m - a, b);
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* a(n) = Q_{n+1} + 2*Q_n + Q_{n-1}  where  Q_j is coeff_y(n,j) */
    seq_t q_n_plus1   = coeff_y(n, n + 1);
    seq_t q_n         = coeff_y(n, n);
    seq_t q_n_minus1  = coeff_y(n, n - 1);

    return q_n_plus1 + (q_n << 1) + q_n_minus1;
}
