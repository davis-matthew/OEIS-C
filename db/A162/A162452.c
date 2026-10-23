/*
 * A162452  Number of reduced words of length n in the Weyl group D_46.
 * https://oeis.org/A162452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ((1 - x^46) * ∏_{i=1}^{45} (1 -
 *            x^{2i})) / (1 - x)^46
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

#define M 46
#define NUM_DEG (M * M)

/* static tables and state */
static bool initialized = false;
static int8_t coeff[NUM_DEG + 1];

/* Compute the signed coefficients of the numerator polynomial. */
static void init_coeff(void)
{
    coeff[0] = 1;
    int cur_deg = 0;

    /* factor (1 - x^M) */
    for (int j = cur_deg; j >= 0; --j) {
        coeff[j + M] -= coeff[j];
    }
    cur_deg += M;

    /* factors (1 - x^{2i}), i = 1 .. M-1 */
    for (int i = 1; i <= M - 1; ++i) {
        int e = 2 * i;
        for (int j = cur_deg; j >= 0; --j) {
            coeff[j + e] -= coeff[j];
        }
        cur_deg += e;
    }
    (void)cur_deg; /* silence unused warning; cur_deg == NUM_DEG after loop */
    initialized = true;
}

/* Public function, external linkage */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_coeff();
    }

    seq_t sum = 0;
    const idx_t kBinom = M - 1;   /* choose M-1 */
    const idx_t offset = kBinom;  /* a = (n - k) + offset */

    for (int k = 0; k <= NUM_DEG && (idx_t)k <= n; ++k) {
        int8_t c = coeff[k];
        if (c == 0) {
            continue;
        }
        idx_t a_val = (n - (idx_t)k) + offset;
        seq_t term = binom(a_val, kBinom);
        if (c > 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return sum;
}
