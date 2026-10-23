/*
 * A165987  a(n) is the trace of the matrix f(X + n*f(X))/f(X), where X is the
 *          2 X 2 matrix [-13, 17; 31, 97] and f(x) = x^3 - 5*x + 67.
 * https://oeis.org/A165987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165987:  a(n) = 2 * (1*C(n+3,3) + 549629409347*C(n+2,3) +
   2197877953721*C(n+1,3) + 549309615337*C(n,3))
   where C(k,3)=k*(k-1)*(k-2)/6.  Index n starts at 1. */
#include <stdint.h>

static inline seq_t binom3(seq_t k)
{
    if (k < 3) return 0;
    return (k * (k - 1) * (k - 2)) / 6;
}

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;

    /* precompute binomial coefficients */
    seq_t t0 = binom3(s + 3);   /* C(n+3,3) */
    seq_t t1 = binom3(s + 2);   /* C(n+2,3) */
    seq_t t2 = binom3(s + 1);   /* C(n+1,3) */
    seq_t t3 = binom3(s);       /* C(n,3)   */

    /* coefficients from the generating function numerator */
    static const seq_t P0 = (seq_t)1;
    static const seq_t P1 = (seq_t)549629409347ULL;
    static const seq_t P2 = (seq_t)2197877953721ULL;
    static const seq_t P3 = (seq_t)549309615337ULL;

    seq_t sum = 0;
    sum += P0 * t0;
    sum += P1 * t1;
    sum += P2 * t2;
    sum += P3 * t3;
    sum *= 2;

    return sum;
}
