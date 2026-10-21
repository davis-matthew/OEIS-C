/*
 * A068518  The sequence S(n,-3,1,1), where S(n,k,t,q) is defined by
 *          Sum_{j=0..n} binomial(n+q,j)^t * B(j,k) and B(j,k) is the j-th
 *          k-poly-Bernoulli number.
 * https://oeis.org/A068518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n+1,k) * * Sum_{j=0..k}
 *            (j+1)^3 * Sum_{i=0..j} (-1)^i * binomial(j,i) * i^k * The
 *            implementation evaluates the inner sums via the identity *
 *            Sum_{i=0}^j (-1)^i binomial(j,i) i^k = (-1)^j * j! * S(k,j) *
 *            where S(k,j) are Stirling numbers of the second kind. Al...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binomial coefficient utility from the build's utils */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Alias for small integer indices */
    idx_t N = n;

    /* Pre‑compute factorials up to N */
    seq_t *fact = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!fact) return (seq_t)0;
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Allocate Stirling number rows */
    seq_t *prev = (seq_t *)calloc((N + 1), sizeof(seq_t));
    seq_t *next = (seq_t *)calloc((N + 1), sizeof(seq_t));
    if (!prev || !next) {
        free(fact);
        free(prev);
        free(next);
        return (seq_t)0;
    }
    prev[0] = (seq_t)1;   /* S(0,0) = 1 */

    /* Result accumulators: positive and negative parts */
    seq_t result_pos = (seq_t)0;
    seq_t result_neg = (seq_t)0;

    /* Iterate over k = 0 .. N */
    for (idx_t k = 0; k <= N; ++k) {
        /* Inner sums for this k: split into even/odd l contributions */
        seq_t inner_pos = (seq_t)0;
        seq_t inner_neg = (seq_t)0;

        for (idx_t l = 0; l <= k; ++l) {
            /* term = (l+1)^3 * fact[l] * S(k,l) */
            seq_t term = prev[l];
            term = term * fact[l];
            seq_t inc = (seq_t)(l + 1);
            term = term * inc * inc * inc;   /* (l+1)^3 */

            /* Add to positive or negative accumulator */
            if (l & 1)
                inner_neg += term;
            else
                inner_pos += term;
        }

        /* Compute binomial(n+1, k) */
        seq_t bin = (seq_t)binomial(n + 1, k);

        /* Update final result: sign (-1)^k */
        if ((k & 1) == 0) {          /* k even: +inner_pos - inner_neg */
            result_pos += bin * inner_pos;
            result_neg += bin * inner_neg;
        } else {                     /* k odd : -inner_pos + inner_neg */
            result_pos += bin * inner_neg;
            result_neg += bin * inner_pos;
        }

        /* Prepare next Stirling row if needed */
        if (k < N) {
            /* S(k+1,l) = l * S(k,l) + S(k,l-1) */
            for (idx_t l = 0; l <= k + 1; ++l) {
                seq_t val = (seq_t)0;
                if (l <= k)
                    val += (seq_t)l * prev[l];
                if (l > 0)
                    val += prev[l - 1];
                next[l] = val;
            }
            /* Swap prev and next for next iteration */
            seq_t *tmp = prev;
            prev = next;
            next = tmp;
        }
    }

    /* Clean up */
    free(fact);
    free(prev);
    free(next);

    /* Final answer: positive part minus negative part */
    return result_pos - result_neg;
}
