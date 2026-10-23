/*
 * A121312  Number of pairs of probabilistically independent subsets in a set
 *          composed of n elements.
 * https://oeis.org/A121312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{u=0..n} Sum_{(a,b) in [u,n] : ab=nu}
 *            C(n,a)*C(a,u)*C(n-a,b-u)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes row n of the binomial triangle iteratively and
 * visits all pairs (a,b) with ab divisible by n by stepping b in multiples of
 * n/gcd(a,n).  All operations are performed in seq_t (arbitrary-precision
 * or typedefed to an integral type).  It works for any seq_t whose arithmetic
 * operators are defined for + - * /. */

#include <stdint.h>
#include <stdbool.h>

/* helper: compute gcd for idx_t */
static inline idx_t gcd_u64(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* helper: compute binomial coefficient C(n,k).  Uses multiplicative formula
 * which never produces non‑integral intermediate values. */
static inline seq_t binom_calc(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;            /* take advantage of symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t total = 0;
    seq_t choose_n_a = 1;                     /* C(n,0) */

    for (idx_t a = 0; a <= n; ++a) {
        /* gcd(a,n) – special case a==0 */
        idx_t g = (a == 0) ? n : gcd_u64(a, n);
        idx_t nprime = n / g;                 /* step size for b */

        /* iterate over b such that a*b divisible by n */
        for (idx_t b = 0; b <= n; b += nprime) {
            idx_t u = (idx_t)(((seq_t)a * (seq_t)b) / (seq_t)n);  /* ab/n */
            /* u is always integer and satisfies 0 <= u <= a, u <= b */
            idx_t diff = b - u;              /* b-u */

            /* compute the three binomial factors */
            seq_t term1 = binom_calc(a, u);
            seq_t term2 = binom_calc(n - a, diff);

            /* assemble the contribution */
            seq_t term = choose_n_a * term1;
            term *= term2;
            total += term;
        }

        /* update choose_n_a for next a (C(n,a+1)) */
        if (a < n) {
            choose_n_a = choose_n_a * (n - a) / (a + 1);
        }
    }

    return total;
}
