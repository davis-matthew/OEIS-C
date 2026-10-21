/*
 * A227710  The Wiener index of the nanostar dendrimer defined pictorially in
 *          Fig. 2 of the Madanshekaf reference.
 * https://oeis.org/A227710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 22, u128 n <= 53,
 *            big any n
 * Helpers:   pow2_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A227710 – Wiener index of the nanostar dendrimer
    a(n) = 4^n * (31752*n - 46494) + 2^n * (3276*n + 51786) - 3996
    (offset 1; a(1) = 47160)                                                */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2_seq.h"

/* helper : compute 2^exp   */

/* helper : compute 4^exp   */
static seq_t pow4_seq(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= 4;
    return r;
}

seq_t a(idx_t n)
{
    /* compute 2^n and 4^n  (exact in seq_t, overflow allowed) */
    seq_t pow2n = pow2_seq(n);
    seq_t pow4n = pow4_seq(n);

    /* integer coefficients, fit in 64‑bit signed */
    int64_t coeff1_signed = (int64_t)31752 * (int64_t)n - 46494;
    int64_t coeff2_signed = (int64_t)3276  * (int64_t)n + 51786;

    /* work with absolute value of coeff1, note its sign separately */
    seq_t abs_coeff1 = (seq_t)(coeff1_signed >= 0 ? coeff1_signed
                                            : -coeff1_signed);
    bool coeff1_negative = (coeff1_signed < 0);

    seq_t term1 = pow4n * abs_coeff1;
    seq_t term2 = pow2n * (seq_t)coeff2_signed;

    seq_t result;
    if (coeff1_negative)
        result = term2 - term1;   /* subtraction is safe: term2 ≥ term1 + 3996 for n ≥ 1 */
    else
        result = term2 + term1;

    result -= 3996;
    return result;
}
