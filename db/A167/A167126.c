/*
 * A167126  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of reduced words of length n in the Coxeter group
 * with 19 generators and relations (S_i)^2 = (S_i S_j)^14 = I.
 *
 * The generating function is
 *   G(t) = (1 + 2t + 2t^2 + … + 2t^13 + t^14) /
 *          (1 - 17t - 17t^2 - … - 17t^13 + 153t^14).
 *
 * Hence the coefficients a(n) satisfy
 *
 *   a(0) = 1,
 *   for 1 ≤ n ≤ 14:   a(n) = 17*∑_{k=1}^{n} a(n-k)   + B_n,
 *                     where B_0 = 1, B_1 … B_13 = 2, B_14 = 1,
 *   for n ≥ 15:       a(n) = 17*∑_{k=1}^{13} a(n-k) - 153*a(n-14).
 *
 * This implementation caches the sequence in a dynamic array so that
 * multiple calls for arbitrary n are efficient.
 */

#include <stddef.h>
#include <stdlib.h>

/* Sequence entry function */
seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *seq = NULL;
    static size_t cached = 0;     /* number of terms stored (cached = last index + 1) */

    /* Ensure capacity for indices up to n */
    if (cached <= (size_t)n) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        if (!tmp) {
            /* memory allocation failed – return zero */
            return (seq_t)0;
        }
        seq = tmp;
        /* initialize newly allocated slots to zero */
        for (size_t i = cached; i < newcap; ++i)
            seq[i] = (seq_t)0;
        cached = newcap;
    }

    /* Numerator coefficients B[0..14] */
    static const seq_t B[15] = {
        1,  /* B0 */
        2,  /* B1 */
        2,  /* B2 */
        2,  /* B3 */
        2,  /* B4 */
        2,  /* B5 */
        2,  /* B6 */
        2,  /* B7 */
        2,  /* B8 */
        2,  /* B9 */
        2,  /* B10 */
        2,  /* B11 */
        2,  /* B12 */
        2,  /* B13 */
        1   /* B14 */
    };

    /* Compute missing entries up to n */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        /* Skip if already computed */
        if (i < cached && seq[i] != (seq_t)0)
            continue;

        seq_t value = (seq_t)0;
        /* sum_{j=1}^{13} 17*a(i-j)  (missing terms are zero) */
        for (size_t j = 1; j <= 13; ++j) {
            if (i >= j)
                value += (seq_t)17 * seq[i - j];
        }
        if (i >= 14)
            value -= (seq_t)153 * seq[i - 14];

        /* add numerator term B_i for i ≤ 14 */
        if (i <= 14 && B[i] != (seq_t)0)
            value += B[i];

        seq[i] = value;
    }

    return seq[(size_t)n];
}
