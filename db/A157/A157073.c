/*
 * A157073  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 48.
 * https://oeis.org/A157073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n,24) T(n,k) = Σ_{i=1..n} binomial(n+1,i) *
 *            binomial(k-1,i-1) * binomial(n-i+k,k) For this sequence k = 24
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 58,
 *            big any n
 * Helpers:   binom_small_24
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom_small_24.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* binomial(23,i) for i = 0..23, cached once */
    static seq_t binom23[24];
    static bool init = false;
    if (!init) {
        binom23[0] = 1;
        for (idx_t i = 1; i <= 23; ++i)
            binom23[i] = binom23[i - 1] * (seq_t)(23 - (i - 1)) / i;
        init = true;
    }

    seq_t sum = 0;
    seq_t C_n1_i = 0;              /* binomial(n+1,i) */
    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1)
            C_n1_i = (seq_t)(n + 1);
        else
            C_n1_i = C_n1_i * (seq_t)(n + 2 - i) / i;

        if (i - 1 > 23)       /* binomial(23,i-1) = 0 for i>24 */
            continue;

        seq_t part1 = binom23[i - 1];                /* binomial(23,i-1) */
        idx_t N = n - i + 24;                       /* N >= 24 */
        seq_t part3 = binom_small_24(N);            /* binomial(N,24) */

        seq_t term = C_n1_i * part1;
        term = term * part3;
        sum += term;
    }
    return sum;
}
