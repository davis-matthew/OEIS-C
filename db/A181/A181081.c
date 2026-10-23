/*
 * A181081  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k)^(n-2k*+1) * n/(n-k).
 * https://oeis.org/A181081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k)^(n-2k+1) * n/(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* unused: sequence starts at 1 */
    seq_t sum = 0;
    idx_t kmax = n >> 1;                     /* floor(n/2) */
    for (idx_t k = 0; k <= kmax; ++k) {
        /* base = binomial(n-k, k) */
        seq_t base = binomial(n - k, k);

        /* exponent = n - 2*k + 1 */
        idx_t exp_idx = n - (k << 1) + 1;
        seq_t exp = (seq_t)exp_idx;

        /* power = base^exp */
        seq_t power = big_pow(base, exp);

        /* numerator = n * power */
        seq_t numerator = ((seq_t)n) * power;

        /* denominator = n - k */
        idx_t denom = n - k;

        /* term = numerator / denom (exact) */
        seq_t term = numerator / denom;

        sum += term;
    }
    return sum;
}
