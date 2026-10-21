/*
 * A071752  Expansion of (1+x^4*C^3)*C^3, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, for n>1
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 3*binomial(2n,n)*(17*n^3+14*n^2-13*n+14)
/         -------------------------------------- */
/*             2*(n+1)*(n+2)*(n+3)*(2n-1)        */

#include <stdbool.h>

/* Euclidean gcd for seq_t */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* binomial( n, k ) using multiplicative formula with reductions
   to prevent overflow.  idx_t is unsigned. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        /* cancel common factors between num and den */
        seq_t g = seq_gcd(num, den);
        num /= g;
        den /= g;

        /* cancel factors that already exist in res */
        g = seq_gcd(res, den);
        res /= g;
        den /= g;

        /* den should now be 1 */
        res *= num;
        /* (optional) assert(den == (seq_t)1); */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)3;

    /* compute binomial(2n, n) */
    seq_t binom = binomial((idx_t)(2 * n), n);

    /* polynomial part: 17*n^3 + 14*n^2 - 13*n + 14 */
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t poly = (seq_t)17 * n3 + (seq_t)14 * n2 - (seq_t)13 * n_seq + (seq_t)14;

    /* denominator: 2*(n+1)*(n+2)*(n+3)*(2*n-1) */
    seq_t den = (seq_t)2
                * (n_seq + (seq_t)1)
                * (n_seq + (seq_t)2)
                * (n_seq + (seq_t)3)
                * (seq_t)((seq_t)2 * n_seq - (seq_t)1);

    /* reduce by gcd to keep numbers small */
    seq_t g = seq_gcd(binom, den);
    binom /= g;
    den /= g;

    /* multiply by 3, reduce with denom */
    binom = binom * (seq_t)3;
    g = seq_gcd(binom, den);
    binom /= g;
    den /= g;

    /* multiply by poly, reduce with denom */
    binom = binom * poly;
    g = seq_gcd(binom, den);
    binom /= g;
    den /= g;

    /* after reductions den must be 1 */
    return binom / den;
}
