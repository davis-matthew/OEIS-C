/*
 * A037963  a(n) = n^2*(n+1)*(3*n^2 + 7*n - 2)*(n+5)!/11520.
 * https://oeis.org/A037963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n+1)*(3*n^2 + 7*n - 2)*(n+5)!/11520
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   factorial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/gcd.h"

/* helper that reduces a factor by the current denominator D and returns the reduced factor.
   It also updates D to reflect the reduction. */
static seq_t reduce_factor(seq_t f, seq_t *D)
{
    seq_t g = gcd(f, *D);
    if (g != 0)
    {
        f  /= g;
        *D /= g;
    }
    return f;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t N   = (seq_t)n;           /* original n */
    seq_t n1  = N + 1;               /* n+1 */
    seq_t poly = 3 * N * N + 7 * N - 2; /* 3*n^2 + 7*n – 2 */
    seq_t fac  = factorial(n + 5);   /* (n+5)! */

    seq_t D = 11520;                 /* denominator */

    seq_t f1 = reduce_factor(N * N, &D);   /* n^2 */
    seq_t f2 = reduce_factor(n1, &D);      /* n+1 */
    seq_t f3 = reduce_factor(poly, &D);    /* 3*n^2 + 7*n – 2 */
    seq_t f4 = reduce_factor(fac, &D);     /* (n+5)! */

    /* After reductions D should be 1; multiply the reduced factors. */
    seq_t result = f1;
    result *= f2;
    result *= f3;
    result *= f4;

    return result;
}
