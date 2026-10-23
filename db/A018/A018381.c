/*
 * A018381  Divisors of 297.
 * https://oeis.org/A018381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 297 (sorted ascending). */
/* Computed by enumerating all divisors of 297 = 3^3 * 11^1. */
/* Returns the n‑th smallest divisor for n ≥ 1; for n > 8 returns 0. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset or beyond the divisor list. */
    if (n == 0 || n > 8)
        return (seq_t)0;

    /* Generate the 8 divisors of 297 on the fly. */
    const seq_t pow3[4]  = {(seq_t)1, (seq_t)3, (seq_t)9,  (seq_t)27};
    const seq_t pow11[2] = {(seq_t)1, (seq_t)11};

    seq_t divs[8];
    size_t k = 0;
    for (size_t e3 = 0; e3 < 4; ++e3)
        for (size_t e11 = 0; e11 < 2; ++e11)
            divs[k++] = pow3[e3] * pow11[e11];

    /* Simple insertion sort on the tiny array. */
    for (size_t i = 0; i < 8; ++i)
        for (size_t j = i + 1; j < 8; ++j)
            if (divs[j] < divs[i]) {
                seq_t tmp   = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }

    /* Return the (n‑1)-th element (zero‑based index). */
    return divs[(size_t)(n - 1)];
}
