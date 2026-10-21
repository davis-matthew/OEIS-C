/*
 * A055855  Convolution of A055854 with A011782.
 * https://oeis.org/A055855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^{min(8,n-1)} (-1)^i * C(8,i) * C(8 + n - i - 1, n
 *            - i - 1) * 2^{n-i-1} where the binomial coefficient C(8 + k, k)
 *            is computed as Π_{t=1}^{8} (k + t) / t
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 42, u128 n <= 98,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function assumes the presence of the utility
   function big_pow(seq_t base, seq_t exp) which returns
   base^exp as a seq_t value. */

#include "../utils/big_pow.h"

static const unsigned int C8[9] = {
    1, 8, 28, 56, 70, 56, 28, 8, 1
};

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    idx_t max_i = n - 1;
    if (max_i > 8)
        max_i = 8;

    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;

    for (idx_t i = 0; i <= max_i; ++i) {
        idx_t k = n - i - 1;              /* k >= 0 */
        seq_t k_seq = (seq_t)k;

        /* Compute binom(8 + k, k) = product_{t=1}^8 (k + t) / t */
        seq_t bin = (seq_t)1;
        for (idx_t t = 1; t <= 8; ++t) {
            bin = bin * (k_seq + (seq_t)t);
            bin = bin / (seq_t)t;
        }

        /* Compute 2^k */
        seq_t pow2 = big_pow((seq_t)2, (seq_t)k);

        /* term = C8[i] * bin * pow2 */
        seq_t term = (seq_t)C8[i] * bin;
        term = term * pow2;

        if ((i & 1) == 0)   /* (-1)^i positive when i even */
            sum_pos += term;
        else
            sum_neg += term;
    }

    /* The result is non‑negative; sum_pos should dominate sum_neg. */
    return sum_pos - sum_neg;
}
