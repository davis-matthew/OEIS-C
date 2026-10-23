/*
 * A173850  Number of permutations of 1..n with no adjacent pair summing to
 *          n+10.
 * https://oeis.org/A173850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..floor((n-9)/2)} (-2)^j *
 *            binomial(floor((n-9)/2), j) * (n-j)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* factorial cache */
    static seq_t *fact = NULL;
    static idx_t fact_size = 0;    /* number of stored entries, indices 0..fact_size-1 */

    if (fact_size == 0) {
        fact = (seq_t*)calloc(1, sizeof(seq_t));
        fact[0] = 1;
        fact_size = 1;
    }
    if ((idx_t)n >= fact_size) {
        idx_t new_size = (idx_t)n + 1;
        fact = (seq_t*)realloc(fact, new_size * sizeof(seq_t));
        for (idx_t i = fact_size; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_size = new_size;
    }

    idx_t m = (n >= 9) ? (n - 9) / 2 : (idx_t)0;

    seq_t binom    = 1;  /* C(m, 0) */
    seq_t pow2     = 1;  /* 2^0 */
    seq_t even_sum = 0;
    seq_t odd_sum  = 0;

    for (idx_t j = 0; j <= m; ++j) {
        seq_t term = binom * pow2;
        term *= fact[n - j];

        if ((j & 1) == 0) {
            even_sum += term;
        } else {
            odd_sum += term;
        }

        if (j == m) break;   /* no further updates needed */

        binom = binom * (seq_t)(m - j);
        binom = binom / (seq_t)(j + 1);

        pow2 = pow2 * 2;
    }

    return even_sum - odd_sum;
}
