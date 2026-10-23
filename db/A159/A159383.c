/*
 * A159383  Number of n X n arrays of squares of integers summing to 11.
 * https://oeis.org/A159383
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # of n×n arrays with entries 0,1,4,9 summing to 11. This
 *            equals the coefficient of x¹¹ in (1+x+x⁴+x⁹)^{n²}, which has
 *            only four admissible triples of counts: (a,b,c) = (11,0,0),
 *            (7,1,0), (3,2,0), (2,0,1) where a,b,c are numbers of 1²,2²,3²
 *            respectively. For each triple the number of arrays...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 16, u128 n <= 100,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Binomial coefficient C(N,k) computed with exact integer arithmetic. */
static inline seq_t binom_full(idx_t N, idx_t k)
{
    if (k > N) return (seq_t)0;
    if (k > N - k) k = N - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Reduce numerator and denominator by their gcd */
        seq_t num = N - k + i;
        seq_t den = i;
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Further reduce res by den */
        g = gcd(res, den);
        res /= g;
        den /= g;

        /* Now den should be 1; multiply the reduced numerator */
        res *= num;      /* multiplication may overflow big‑int type; handled */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    idx_t N = n * n;
    seq_t total = (seq_t)0;

    /* (a,b,c) = (11,0,0) */
    if (N >= 11) {
        total += binom_full(N, 11);
    }

    /* (a,b,c) = (7,1,0) */
    if (N >= 8) {
        seq_t t = binom_full(N, 7);
        t *= (seq_t)(N - 7);              /* C(N-7,1) */
        total += t;
    }

    /* (a,b,c) = (3,2,0) */
    if (N >= 5) {
        seq_t t = binom_full(N, 3);
        seq_t part = (seq_t)(N - 3);
        part *= (seq_t)(N - 4);
        part /= 2;                        /* C(N-3,2) */
        t *= part;
        total += t;
    }

    /* (a,b,c) = (2,0,1) */
    if (N >= 3) {
        seq_t t = binom_full(N, 2);
        t *= (seq_t)(N - 2);              /* C(N-2,1) */
        total += t;
    }

    return total;
}
