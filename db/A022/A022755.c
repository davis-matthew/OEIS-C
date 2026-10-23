/*
 * A022755  Expansion of 1/Product_{m>=1} (1 - m*q^m)^31.
 * https://oeis.org/A022755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = coefficient of q^n in * 1 / ∏_{m≥1} (1 - m
 *            q^m)^31 * * The implementation multiplies the
 *            generating‑function factors * one by one, keeping only the first
 *            n+1 coefficients. It works * for any seq_t (integer or
 *            arbitrary‑precision)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 23, u128 n <= 59,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* ------------------------------------------------------------------ */
/*  a(n) – returns the nth term of sequence A022755                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate two double buffers for the dynamic programming */
    seq_t *buf0 = alloc_seq_array((size_t)(n + 1));
    seq_t *buf1 = alloc_seq_array((size_t)(n + 1));
    if (!buf0 || !buf1) {
        free_seq_array(buf0);
        free_seq_array(buf1);
        return 0;               /* allocation failed */
    }

    seq_t *old_buf = buf0;
    seq_t *new_buf = buf1;

    /* Initialise the polynomial to 1 (i.e. coefficient of q^0 = 1) */
    for (size_t i = 0; i <= (size_t)n; ++i)
        old_buf[i] = 0;
    old_buf[0] = 1;

    /* Multiply successively by (1 - m*q^m)^(-31) for m = 1 .. n */
    for (idx_t m = 1; m <= n; ++m)
    {
        /* Start new_buf with the current polynomial */
        for (size_t i = 0; i <= (size_t)n; ++i)
            new_buf[i] = old_buf[i];

        /* Add contributions of higher powers of (1 - m*q^m)^(-31) */
        idx_t maxK = n / m;              /* largest k with k*m <= n */
        seq_t binom = 1;                 /* binom(30+0,0) */
        seq_t powm  = 1;                 /* m^0 */

        for (idx_t k = 1; k <= maxK; ++k)
        {
            /* binom(30+k, k) = binom(30+(k-1), k-1) * (30+k) / k */
            binom = binom * ((seq_t)30 + k) / k;
            powm  = powm * (seq_t)m;
            seq_t coeff = binom * powm;   /* coeff of q^{k*m} in this factor */

            idx_t shift = m * k;
            for (idx_t idx = shift; idx <= n; ++idx)
            {
                new_buf[idx] = new_buf[idx] + old_buf[idx - shift] * coeff;
            }
        }

        /* Prepare for the next iteration */
        seq_t *tmp = old_buf;
        old_buf = new_buf;
        new_buf = tmp;
    }

    seq_t result = old_buf[(size_t)n];

    /* Clean up */
    free_seq_array(buf0);
    free_seq_array(buf1);

    return result;
}
