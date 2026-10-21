/*
 * A239750  Number of ordered pairs of endofunctions (f,g) on a set of n
 *          elements satisfying g(f(x)) = f(f(f(x))).
 * https://oeis.org/A239750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * k^n * (n-1)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A239750.
 * The function accepts n as idx_t and returns the term as seq_t.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h" /* binomial(idx_t, idx_t) */
#include "../utils/pow_seq.h"   /* pow_seq(seq_t, int) */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t, seq_t) */

seq_t a(idx_t n) {
    /* Handle offset n = 0 explicitly. */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = (seq_t)0;
    /* Start with binomial coefficient C(n,0) = 1.
     * It will be updated incrementally to avoid recomputing
     * using factorials. */
    seq_t binom_coef = (seq_t)1; /* equal to C(n,0) */

    /* Precompute base-(n-1) as a seq_t value. */
    seq_t base = (seq_t)(n - 1);

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute k^n and (n-1)^(n-k). */
        seq_t pow_k = pow_seq((seq_t)k, (int)n);
        /* For n-k = 0 the exponent 0 → result 1. */
        seq_t pow_base = pow_seq(base, (int)(n - k));

        /* Multiply: term = binom_coef * pow_k * pow_base */
        seq_t term = seq_mul(binom_coef, pow_k);
        term = seq_mul(term, pow_base);

        /* Accumulate into sum. */
        sum += term; /* + is defined for both unsigned and arbitrary‑precision types */

        /* Update binomial coefficient for next k:
         * C(n,k+1) = C(n,k) * (n-k) / (k+1)   (only executed if k < n) */
        if (k < n) {
            /* temp = binom_coef * (n - k);   // multiplication */
            seq_t temp = seq_mul(binom_coef, (seq_t)(n - k));
            /* binom_coef = temp / (k + 1);   // integer division */
            /* We rely on natural truncation of the division for integral types;
             * the division is exact because binom_coef is guaranteed to be divisible. */
#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
            binom_coef = temp / ((seq_t)(k + 1));
#else
            /* For other builds where seq_t is unsigned integer or class, we convert to seq_t. */
            binom_coef = temp / ((seq_t)(k + 1));
#endif
        }
    }

    return sum;
}
