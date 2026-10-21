/*
 * A083539  a(n) = sigma(n) * sigma(n+1): product of sigma-values for
 *          consecutive integers.
 * https://oeis.org/A083539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) * sigma(n+1) (product of divisor sums of n and
 *            n+1)
 * Range:     u8 n <= 10, u16 n <= 142, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for σ values */
static seq_t *sigma = NULL;      /* σ[0] is unused */
static idx_t sigma_max = 0;      /* maximum index that has been computed */

/* Ensure σ values are available up to and including idx (i.e., σ[0..idx]).  */
static void ensure_sigma(idx_t idx)
{
    if (idx <= sigma_max) return;          /* already computed */

    /* Allocate or grow the array */
    size_t new_size = (size_t)idx + 1;      /* need indices 0 .. idx */
    if (sigma == NULL) {
        sigma = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!sigma) return;                /* allocation failure – abort gracefully */
        for (size_t i = 0; i < new_size; ++i) sigma[i] = 0;
    } else {
        sigma = (seq_t *)realloc(sigma, new_size * sizeof(seq_t));
        if (!sigma) return;
        for (size_t i = (size_t)sigma_max + 1; i < new_size; ++i) sigma[i] = 0;
    }

    /* Add contributions from new divisors for indices > sigma_max */
    for (size_t d = 1; d < new_size; ++d) {
        /* Find first multiple of d that is greater than old max */
        size_t start = ((size_t)sigma_max + d) / d;
        start *= d;
        while (start < new_size) {
            sigma[start] += (seq_t)d;
            start += d;
        }
    }

    sigma_max = idx;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* We need σ(n) and σ(n+1) */
    ensure_sigma(n + 1);
    return sigma[(size_t)n] * sigma[(size_t)n + 1];
}
