/*
 * A238636  Number of partitions of 9^n into parts that are at most 9.
 * https://oeis.org/A238636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of 9^n into parts at most 9
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_seq.h"   /* provides seq_t pow_seq(seq_t base, int exp) */

static bool initialized = false;
static seq_t diff[2520][9];   /* Binomial‑basis coefficients for each residue */

/* Precompute all coefficients once */
static void init_coeffs(void)
{
    if (initialized) return;
    const idx_t MAX_M = 22680;            /* 2520 * 9 */
    seq_t dp[MAX_M + 1];
    dp[0] = 1;                            /* Only one way to make 0 */

    /* Unbounded knapsack (coin change) for parts 1..9 */
    for (idx_t coin = 1; coin <= 9; ++coin) {
        for (idx_t amt = coin; amt <= MAX_M; ++amt) {
            dp[amt] += dp[amt - coin];
        }
    }

    /* For each residue r modulo 2520, compute the 9 values and their
       finite differences (Newton basis coefficients). */
    for (idx_t r = 0; r < 2520; ++r) {
        seq_t vals[9];
        for (idx_t t = 0; t < 9; ++t) {
            idx_t m = r + t * 2520;
            vals[t] = dp[m];
        }

        /* Compute successive finite differences in place */
        seq_t temp[9];
        for (idx_t i = 0; i < 9; ++i) temp[i] = vals[i];

        seq_t coeff[9];
        coeff[0] = temp[0];
        for (idx_t k = 1; k < 9; ++k) {
            for (idx_t i = 0; i < 9 - k; ++i) {
                temp[i] = temp[i + 1] - temp[i];
            }
            coeff[k] = temp[0];
        }

        for (idx_t k = 0; k < 9; ++k) diff[r][k] = coeff[k];
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init_coeffs();                       /* Ensure coefficients are ready */

    /* Compute m = 9^n */
    seq_t m = pow_seq((seq_t)9, (int)n);

    /* Residue and quotient for binomial basis */
    seq_t r = m % (seq_t)2520;
    idx_t r_idx = (idx_t)r;                     /* 0 <= r < 2520 */
    seq_t t = (m - r) / (seq_t)2520;            /* t may be astronomically large */

    /* Evaluate polynomial g(t) = Σ binom(t,i) * diff[r][i]  (i=0..8) */
    seq_t value = 0;
    seq_t binom = 1;                            /* binom(t,0) */

    for (idx_t i = 0; i < 9; ++i) {
        if (i > 0) {
            seq_t num = t - (seq_t)(i - 1);
            binom = (binom * num) / (seq_t)i;    /* exact division */
        }
        value = value + binom * diff[r_idx][i];
    }
    return value;
}
