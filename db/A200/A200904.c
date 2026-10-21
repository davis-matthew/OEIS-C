/*
 * A200904  E.g.f. A(x) satisfies: A(x) = x*(2*exp(A(x)) - exp(2*A(x))).
 * https://oeis.org/A200904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{j=0..n} 2^j*(2*n-j)^(n-1)*(-1)^(n-j)*binomial(n,j),
 *            n > 0, a(0)=0
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* computes base^exp for seq_t values (exp fits in idx_t) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp)
    {
        if (exp & 1)
            result = result * cur;
        cur = cur * cur;
        exp >>= 1;
    }
    return result;
}

/* sequence term a(n) : n>=1, a(0)=0 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* outside the offset */

    seq_t sum   = 0;
    seq_t bin   = 1;                    /* C(n,0) */
    seq_t pow2  = 1;                    /* 2^0 */
    idx_t exp   = n - 1;                /* exponent in (2*n-j)^(n-1) */

    for (idx_t j = 0; j <= n; ++j)
    {
        seq_t base = ((seq_t)2 * (seq_t)n) - (seq_t)j;  /* 2*n-j */
        seq_t pow_base = pow_seq(base, exp);            /* (2*n-j)^(n-1) */

        seq_t term = bin * pow2 * pow_base;             /* C(n,j)*2^j*(2n-j)^(n-1) */

        /* apply the sign (-1)^(n-j) */
        if ( ((n - j) & 1) != 0 )
            term = -term;

        sum += term;

        if (j == n) break;                             /* loop end */

        /* Next binomial C(n,j+1) = C(n,j)*(n-j)/(j+1) */
        bin = bin * ((seq_t)n - (seq_t)j) / (seq_t)(j + 1);

        /* Next 2^j */
        pow2 *= (seq_t)2;
    }

    return sum;
}
