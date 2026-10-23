/*
 * A133727  A051731 * A007438 as a diagonalized matrix.
 * https://oeis.org/A133727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = d_j if j divides i else 0; where (i,j) are such that n =
 *            T(i‑1) + j, T(k) = k(k+1)/2, and d_j = Σ_{d|j} μ(d) * T(j/d)
 *            (inverse Möbius transform of the triangular numbers)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Integer square root: returns floor(sqrt(n)) for 0 <= n < 2^64. */
static idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid <= n / mid) {          /* mid*mid <= n, no overflow */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* Global cache for Möbius function and A007438 values. */
static idx_t d_cap = 0;          /* largest index computed so far (inclusive) */
static int8_t *mu = NULL;        /* Möbius values: -1, 0, +1 */
static int64_t *d_int = NULL;    /* A007438 values (signed 64‑bit) */

/* Recompute Möbius array μ[1..limit] and A007438 array d_int[1..limit]. */
static void recompute_arrays(idx_t limit)
{
    /* Allocate new μ array */
    int8_t *new_mu = (int8_t *)calloc((size_t)limit + 1, sizeof(int8_t));
    if (!new_mu) abort();

    bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_comp) abort();
    int *pr = (int *)malloc((size_t)limit * sizeof(int));
    if (!pr) abort();
    size_t pcnt = 0;

    new_mu[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            pr[pcnt++] = (int)i;
            new_mu[i] = -1;
        }
        for (size_t j = 0; j < pcnt && (idx_t)pr[j] * i <= limit; ++j) {
            idx_t x = (idx_t)pr[j] * i;
            is_comp[x] = true;
            if (i % pr[j] == 0) {
                new_mu[x] = 0;
                break;
            } else {
                new_mu[x] = -new_mu[i];
            }
        }
    }
    free(is_comp);
    free(pr);

    /* Allocate new A007438 array */
    int64_t *new_d = (int64_t *)calloc((size_t)limit + 1, sizeof(int64_t));
    if (!new_d) abort();

    /* Dirichlet convolution: d[m] = Σ_{d|m} μ(d)*T(m/d) */
    for (idx_t dval = 1; dval <= limit; ++dval) {
        if (new_mu[dval] == 0) continue;
        int64_t mu_val = new_mu[dval] > 0 ? 1 : -1;
        for (idx_t m = dval; m <= limit; m += dval) {
            idx_t k = m / dval;
            int64_t tri = (int64_t)k * ( (int64_t)k + 1 ) / 2;
            new_d[m] += mu_val * tri;
        }
    }

    /* Replace old arrays */
    free(mu);
    free(d_int);
    mu = new_mu;
    d_int = new_d;
    d_cap = limit;
}

/* Ensure that d_int is ready up to at least 'n' (inclusive). */
static void ensure_d(idx_t n)
{
    if (n <= d_cap) return;
    idx_t new_cap = d_cap == 0 ? 1 : d_cap;
    while (new_cap < n) new_cap <<= 1;
    recompute_arrays(new_cap);
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, return 0 */

    /* Find row i such that T(i-1) < n <= T(i). */
    idx_t disc = floor_isqrt_u64(1 + 8 * n);
    idx_t j = (disc - 1) >> 1;          /* floor of (-1 + sqrt)/2 */
    idx_t tri_j = j * (j + 1) / 2;
    idx_t i = (tri_j >= n) ? j : j + 1;

    /* Column index within the row. */
    idx_t tri_prev = (i - 1) * i / 2;
    idx_t col = n - tri_prev;           /* 1 <= col <= i */

    /* Ensure we have the A007438 values up to 'col'. */
    ensure_d(col);

    if (i % col != 0) return 0;
    return (seq_t)d_int[col];  /* cast to the appropriate integer type */
}
