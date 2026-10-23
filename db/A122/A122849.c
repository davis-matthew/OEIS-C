/*
 * A122849  Diagonal sums of Bessel related triangle A122848.
 * https://oeis.org/A122849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..floor(n/2)} k! * binom(n-k,k) / ((3k-n)! *
 *            2^{n-2k})
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are provided by the build environment. */
static seq_t *fact = NULL;
static idx_t fact_size = 0;

/* Ensure factorial table contains values up to m. */
static void ensure_fact(idx_t m)
{
    if (m < fact_size)
        return;
    size_t new_size = m + 1;
    seq_t *new_fact = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_fact)
        return; /* allocation failure; let the caller handle it */
    if (!fact) {
        new_fact[0] = 1;
        for (idx_t i = 1; i <= m; ++i)
            new_fact[i] = new_fact[i - 1] * (seq_t)i;
    } else {
        memcpy(new_fact, fact, fact_size * sizeof(seq_t));
        for (idx_t i = fact_size; i <= m; ++i)
            new_fact[i] = new_fact[i - 1] * (seq_t)i;
        free(fact);
    }
    fact = new_fact;
    fact_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_fact(n);
    seq_t sum = 0;
    idx_t k_start = (n + 2) / 3;      /* ceil(n/3) */
    idx_t k_end   = n / 2;            /* floor(n/2) */
    if (k_start > k_end)
        return sum;

    for (idx_t k = k_start; k <= k_end; ++k) {
        /* numerator = k! * C(n-k, k) */
        seq_t fact_k = fact[k];
        idx_t a_val = n - k;                 /* a = n - k in binomial */
        seq_t bin = 1;
        for (idx_t i = 1; i <= k; ++i)
            bin = (bin * (a_val - i + 1)) / i;
        seq_t numerator = fact_k * bin;

        /* denominator = (3k-n)! * 2^(n-2k) */
        idx_t d_fact_idx = 3 * k - n;        /* 3k - n >= 0 */
        seq_t denom_fact = fact[d_fact_idx];
        idx_t exp2 = n - 2 * k;
        seq_t pow2_val = ((seq_t)1) << exp2; /* 2^(n-2k) */
        seq_t denominator = denom_fact * pow2_val;

        seq_t term = numerator / denominator;
        sum += term;
    }
    return sum;
}
