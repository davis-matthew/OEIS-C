/*
 * A086646  Triangle, read by rows, of numbers T(n,k), 0 <= k <= n, given by
 *          T(n,k) = A000364(n-k)*binomial(2*n, 2*k).
 * https://oeis.org/A086646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where (r,k) are the row and column indices of *
 *            the triangle read row‑by‑row. T(r,k) = A000364(r-k) *
 *            binomial(2*r,2*k)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 78, u128 n <= 209,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Cache for the Euler numbers E_{m} = A000364(m) (m = 0..max). */
static seq_t *euler = NULL;          /* array of computed Euler numbers */
static size_t euler_cap = 0;         /* allocated capacity */
static idx_t   euler_max = (idx_t)-1;/* largest index already computed; sentinel */

/* Expand the `euler` array to accommodate indices up to `max`. */
static void ensure_euler(idx_t max)
{
    if (max <= euler_max && euler != NULL)
        return;                     /* already done */

    /* Allocate or re‑allocate sufficient storage. */
    size_t needed = (size_t)(max + 1);
    if (euler_cap < needed) {
        size_t new_cap = euler_cap ? euler_cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(euler, new_cap * sizeof(seq_t));
        euler = tmp;
        euler_cap = new_cap;
    }

    /* Initialise new slots. */
    for (size_t i = euler_max + 1; i < needed; ++i)
        euler[i] = 0;

    /* Ensure base value E_0 = 1. */
    if (euler_max == (idx_t)-1) {
        euler[0] = (seq_t)1;
        euler_max = 0;
    }

    /* Compute Euler numbers for indices euler_max+1 … max. */
    for (idx_t n = euler_max + 1; n <= max; ++n) {
        seq_t pos = 0;   /* sum of positive terms */
        seq_t neg = 0;   /* sum of negative terms */
        for (idx_t k = 0; k < n; ++k) {
            seq_t term = binomial((idx_t)(2 * n), (idx_t)(2 * k)) * euler[k];
            /* sign = (-1)^(n-k+1).  Odd exponent → negative. */
            if (((n - k + 1) & 1ULL) == 1ULL)
                neg += term;
            else
                pos += term;
        }
        /* pos >= neg for all n; result is positive. */
        euler[n] = pos - neg;
    }

    euler_max = max;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1ULL) * (r + 2ULL) / 2ULL <= n)
        ++r;

    /* Column index k. */
    idx_t k = n - r * (r + 1ULL) / 2ULL;   /* 0 ≤ k ≤ r */

    /* Ensure Euler numbers up to r are available. */
    ensure_euler(r);

    /* T(r,k) = E_{r-k} * C(2r,2k). */
    seq_t e_val   = euler[r - k];
    seq_t bin_val = binomial((idx_t)(2 * r), (idx_t)(2 * k));
    return e_val * bin_val;
}
