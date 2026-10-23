/*
 * A132930  Concatenation of first n elements of the sigma function, where
 *          sigma(n) is the sum of divisors of n.
 * https://oeis.org/A132930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concatenation of σ(k) for k = 1..n * where σ(k) = sum of
 *            divisors of k (OEIS A132930)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e);  */
#include "../utils/digit_count.h"    /* int digit_count(uint64_t n); */

/* External function as required */
seq_t a(idx_t n)
{
    /* 0‑based arrays (index 0 unused); cache the computed values */
    static uint64_t *sigma  = NULL;   /* sigma[i] for i ≥ 1, type uint64_t */
    static seq_t    *concat = NULL;   /* concatenated value up to i   */
    static idx_t    current = 0;      /* largest n cached so far      */

    if (n < 1) return 0;               /* only defined for n ≥ 1 */

    /* Extend cache if n exceeds current maximum */
    if (n > current) {
        /* Resize sigma array to n+1 elements (index 0 unused) */
        uint64_t *new_sigma = (uint64_t*)realloc(sigma, sizeof(uint64_t)*(n + 1));
        if (!new_sigma) return 0;   /* allocation failure – fail gracefully */
        sigma = new_sigma;

        /* Initialize new entries to 0 if expanding */
        for (idx_t i = current + 1; i <= n; ++i) sigma[i] = 0;

        /* Compute σ values for all numbers up to n
         * (simple divisor sieve – O(n log n), fast for n≈10⁴ ) */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                sigma[j] += (uint64_t)i;
            }
        }

        /* Resize concat array to n+1 elements */
        seq_t *new_concat = (seq_t*)realloc(concat, sizeof(seq_t)*(n + 1));
        if (!new_concat) return 0;
        concat = new_concat;

        /* Compute concatenated values from previous maximum up to n */
        seq_t value = (current == 0) ? 0 : concat[current];
        for (idx_t i = current + 1; i <= n; ++i) {
            uint64_t  s   = sigma[i];
            int       digs = digit_count(s);        /* decimal digits of σ(i) */
            seq_t     mul  = pow10(digs);           /* 10^digs as seq_t */
            value = value * mul + (seq_t)s;        /* append σ(i) */
            concat[i] = value;
        }
        current = n;       /* update cached maximum */
    }

    return concat[n];
}
