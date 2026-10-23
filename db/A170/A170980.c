/*
 * A170980  Expansion of (Product_{i=1..m-1} (1 + x^(4*i-1))) * (1 +
 *          x^(2*m-1)) for m = 8.
 * https://oeis.org/A170980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170980: Expansion of (Product_{i=1..7} (1 + x^(4*i-1))) * (1 + x^15),
 * with m = 8.  Coefficients are returned for any n ≥ 0;
 * coefficients for n > 120 are zero. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t coeff[121];
static bool initialized = false;

static void init_coeff(void)
{
    /* List of exponents corresponding to the eight factors,
       note that exponent 15 appears twice. */
    const unsigned int exps[8] = {3, 7, 11, 15, 19, 23, 27, 15};
    /* Initialize coefficients */
    for (size_t i = 0; i < 121; ++i) coeff[i] = 0;
    coeff[0] = 1;
    unsigned int cur_max = 0;
    for (size_t e = 0; e < 8; ++e) {
        unsigned int exp = exps[e];
        /* update in reverse to avoid double counting a term within the same factor */
        for (int j = cur_max; j >= 0; --j) {
            seq_t add = coeff[j];
            if (add) {
                seq_t *target = &coeff[j + exp];
                *target = *target + add;
            }
        }
        cur_max += exp;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_coeff();
    if (n > 120) return (seq_t)0;
    return coeff[(size_t)n];
}
