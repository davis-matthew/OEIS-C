/*
 * A173846  Number of permutations of 1..n with no adjacent pair summing to
 *          n+6.
 * https://oeis.org/A173846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..m} (-2)^j * binomial(m,j) * (n-j)! where m =
 *            max(0, floor((n-5)/2))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache of factorials */
    static seq_t *fact = NULL;
    static idx_t fact_size = 0;   /* one past last index stored (fact_size-1 valid) */

    /* extend factorial cache if needed */
    if (n >= fact_size) {
        idx_t new_size = n + 1;
        seq_t *new_fact = (seq_t *)realloc(fact, new_size * sizeof(seq_t));
        if (!new_fact) return 0;   /* allocation failed; caller should handle */

        /* initialize missing factorials */
        if (fact_size == 0) {
            new_fact[0] = (seq_t)1;   /* 0! = 1 */
            fact_size = 1;
        }
        for (idx_t i = fact_size; i <= n; ++i) {
            new_fact[i] = new_fact[i-1] * (seq_t)i;
        }
        fact = new_fact;
        fact_size = new_size;
    }

    /* compute m = floor((n-5)/2) if n>=5 else 0 */
    idx_t m = 0;
    if (n >= 5) m = (n - 5) >> 1;   /* integer division by 2 */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    seq_t power = 1;   /* 2^j */

    for (idx_t j = 0; j <= m; ++j) {
        seq_t term = fact[n - j];          /* (n-j)!    */
        seq_t bin  = binomial(m, j);       /* C(m,j)    */
        term = term * bin;                 /* term = term * C(m,j) */
        term = term * power;               /* term * 2^j */

        if ((j & 1) == 0) {                /* even j -> (+) */
            sum_pos += term;
        } else {                           /* odd j  -> (-) */
            sum_neg += term;
        }

        power *= (seq_t)2;                 /* prepare 2^(j+1) */
    }

    /* final result: sum_pos - sum_neg;  sum_pos >= sum_neg for this sequence */
    seq_t result = sum_pos;
    result -= sum_neg;
    return result;
}
