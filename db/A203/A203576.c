/*
 * A203576  Exponential (or binomial) half-convolution of A000984 (central
 *          binomial) with itself.
 * https://oeis.org/A203576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)*cbi(k)*cbi(n-k) where
 *            cbi(m) = binomial(2m,m)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Copyright   2026  OpenAI   */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate array to hold central binomials cbi[0..n] */
    seq_t *cbi = alloc_seq_array((size_t)n + 1);
    if (!cbi) return 0;  /* allocation failure – unlikely in tests */

    /* cbi[0] = 1 */
    cbi[0] = 1;
    for (idx_t i = 0; i < n; ++i) {
        /* compute cbi[i+1] = cbi[i] * 2*(2*i+1)/(i+1) */
        seq_t mult = (seq_t)(2 * (2 * i + 1));
        seq_t denom = (seq_t)(i + 1);
        cbi[i + 1] = (cbi[i] * mult) / denom;
    }

    seq_t sum = 0;
    seq_t binom_nk = 1;          /* binomial(n,0) */
    idx_t half = n / 2;          /* floor(n/2) */

    for (idx_t k = 0; k <= half; ++k) {
        /* term = binom(n,k) * cbi(k) * cbi(n-k) */
        seq_t term = binom_nk * cbi[k] * cbi[n - k];
        sum += term;

        if (k < half) {
            /* update binom_nk to binomial(n,k+1) */
            seq_t num = binom_nk * (seq_t)(n - k);
            binom_nk = num / (seq_t)(k + 1);
        }
    }

    free_seq_array(cbi);
    return sum;
}
