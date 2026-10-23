/*
 * A123553  A "king chicken" in a tournament graph (a directed labeled graph
 *          on n nodes with a single arc between every pair of nodes) is a
 *          player A who for any other player B either beats B directly or
 *          beats someone who beats B. Sequence gives total number of king
 *          chickens in all 2^(n(n-1)/2) tournaments.
 * https://oeis.org/A123553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=0}^{n-1} C(n-1,k) * 2^{C(k,2)+C(n-1-k,2)} *
 *            (2^k-1)^{n-1-k}
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External utilities assumed to be available */
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for offset 1, but return 0 for safety */
        return 0;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n - 1; ++k) {
        /* binomial coefficient C(n-1,k) */
        seq_t comb = binomial(n - 1, k);

        /* exponent for 2^{C(k,2)+C(n-1-k,2)} */
        idx_t exp_tri_k      = k * (k - 1) / 2;
        idx_t n_minus_1_k    = n - 1 - k;
        idx_t exp_tri_rest  = n_minus_1_k * (n_minus_1_k - 1) / 2;
        idx_t exp_tri_total = exp_tri_k + exp_tri_rest;

        seq_t pow_tri = pow2(exp_tri_total);

        /* base (2^k - 1) */
        seq_t pow_2k = pow2(k);
        seq_t base_minus1 = pow_2k - (seq_t)1;

        /* (2^k - 1)^{n-1-k} */
        seq_t power_rest = big_pow(base_minus1, n_minus_1_k);

        /* term = comb * pow_tri * power_rest */
        seq_t term = comb * pow_tri;
        term *= power_rest;

        sum += term;
    }

    /* final result */
    seq_t result = (seq_t)n * sum;
    return result;
}
