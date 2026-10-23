/*
 * A162365  Number of reduced words of length n in the Weyl group D_23.
 * https://oeis.org/A162365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in (1 + x + … + x^22) * Prod_{i=1..22}
 *            (1 + x + … + x^(2i-1))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 61, u128 n <= 506,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Pre‑compute the coefficients once and store them in a static array.
   The product has degree 506, so we allocate 507 entries. */
static seq_t coeff_cache[507];
static bool coeff_computed = false;

/* Helper that fills coeff_cache with the sequence coefficients. */
static void
compute_coefficients(void)
{
    /* The list of factor lengths:
       f(23) has length 23, and for i = 1..22, f(2i) has length 2i. */
    const size_t factor_lengths[23] = {
        23, /* f(23) */
        2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22,
        24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44
    };
    const size_t NUM_FACTORS = 23U;
    const size_t MAX_DEG = 506U; /* total degree of the product */

    seq_t dp[507];
    seq_t tmp[507];

    /* initialise dp to zero and set dp[0] = 1 */
    for (size_t i = 0; i <= MAX_DEG; ++i) dp[i] = 0;
    dp[0] = 1;

    size_t current_deg = 0;

    for (size_t f = 0; f < NUM_FACTORS; ++f) {
        size_t len = factor_lengths[f];               /* length of this factor */
        size_t new_deg = current_deg + (len - 1);

        /* zero the temporary array up to new_deg */
        for (size_t i = 0; i <= new_deg; ++i) tmp[i] = 0;

        /* convolution: for each existing term dp[j], add it to
           tmp[j+r] for r = 0 .. len-1 */
        for (size_t j = 0; j <= current_deg; ++j) {
            seq_t val = dp[j];
            if (!val) continue;
            for (size_t r = 0; r < len; ++r) {
                tmp[j + r] += val;
            }
        }

        /* copy tmp back into dp for the next iteration */
        for (size_t i = 0; i <= new_deg; ++i) dp[i] = tmp[i];
        current_deg = new_deg;
    }

    /* copy the final coefficients into the cache */
    for (size_t i = 0; i <= MAX_DEG; ++i) coeff_cache[i] = dp[i];
}

/* The required OEIS function. */
seq_t
a(idx_t n)
{
    if (!coeff_computed) {
        compute_coefficients();
        coeff_computed = true;
    }

    /* the polynomial has degree 506; terms beyond that are zero. */
    if (n > 506u) {
        return 0;
    }

    return coeff_cache[(size_t)n];
}
