/*
 * A217914  O.g.f.: Sum_{n>=0} (n^4)^n * exp(-n^4*x) * x^n / n!.
 * https://oeis.org/A217914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Stirling2(4*n, n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   binomial, factorial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the identity S(n,k) = 1/k! * Σ_{j=0..k} (-1)^j * C(k,j) * (k-j)^n */
/*  with n = 4*n and k = n.                               */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t exp = 4 * n;                      /* exponent for powers (k-j)^n          */
    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = binomial(n, j);       /* C(n,j)                              */
        seq_t p    = pow_int((seq_t)(n - j), exp); /* (n-j)^(4*n)           */
        term *= p;
        if (j & 1U)
            sum -= term;                   /* (-1)^j factor                       */
        else
            sum += term;
    }
    seq_t fact = factorial(n);             /* n!                                   */
    return sum / fact;                     /* final result                         */
}
