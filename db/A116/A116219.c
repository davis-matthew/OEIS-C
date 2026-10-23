/*
 * A116219  If X_1,...,X_n is a partition of a 3n-set X into 3-blocks then
 *          a(n) is equal to the number of permutations f of X such that
 *          f(X_i) <> X_i, (i=1,...,n).
 * https://oeis.org/A116219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (-6)^j * binomial(n,j) * (3*n-3*j)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for a 3n‑set partition problem. */

#include <stdlib.h>

/* Cached factorials up to the largest n seen so far.
   fact[i] = i!   (i ranges from 0 to 3*max_n). */
static seq_t *fact = NULL;          /* null until first use */
static idx_t fact_size = 0;         /* number of elements allocated */

/* Ensure factorial table contains values up to 'upto' (inclusive). */
static void ensure_factorial(idx_t upto)
{
    if (fact == NULL) {
        /* first time: allocate and compute factorials */
        fact_size = upto + 1;
        fact = (seq_t *)malloc(fact_size * sizeof(seq_t));
        fact[0] = 1;
        for (idx_t i = 1; i <= upto; ++i)
            fact[i] = fact[i - 1] * i;
    } else if (upto >= fact_size) {
        /* enlarge array */
        idx_t new_size = upto + 1;
        fact = (seq_t *)realloc(fact, new_size * sizeof(seq_t));
        for (idx_t i = fact_size; i <= upto; ++i)
            fact[i] = fact[i - 1] * i;
        fact_size = new_size;
    }
}

/* Main sequence function.  External linkage as required. */
seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n = 0; return 0 as safe default */
        return 0;

    /* Ensure factorial table has the needed range up to 3*n. */
    ensure_factorial(3 * n);

    /* Precompute 6^n for the first term. */
    seq_t pow6 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow6 *= 6;                       /* pow6 = 6^n   */

    /* binomial coefficient C(n, 0) = 1 */
    seq_t binom = 1;

    /* Positive and negative accumulators.  final result = pos - neg. */
    seq_t pos = 0, neg = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = C(n,k) * fact[3*k] * 6^(n-k) with sign (-1)^(n-k). */
        idx_t m = n - k;                      /* exponent of 6 */
        seq_t term = binom * fact[3 * k] * pow6;

        /* Determine sign: negative if m is odd. */
        if (m & 1)
            neg += term;                      /* add to negative part */
        else
            pos += term;                      /* add to positive part */

        /* Prepare for next iteration. */
        if (k != n) {
            /* Update C(n, k+1) = C(n, k) * (n-k)/(k+1). */
            binom = binom * (n - k) / (k + 1);
            /* Divide 6^(n-k) by 6 to get 6^(n-(k+1)). */
            pow6 /= 6;
        }
    }

    /* Since the final value is non‑negative, subtraction will not underflow. */
    return pos - neg;
}
