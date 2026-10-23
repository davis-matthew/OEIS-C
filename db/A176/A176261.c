/*
 * A176261  Triangle T(n,k) = A006130(k) - A006130(n) + A006130(n-k) read by
 *          rows.
 * https://oeis.org/A176261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r,k) with r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2, T(r,k)
 *            = A006130(k) - A006130(r) + A006130(r-k), where A006130(m) =
 *            Σ_{j=0..m} binomial(m-j,j) * 3^j
 * Range:     i8 n <= 29, i16 n <= 105, i32 n <= 378, i64 n <= 1433,
 *            i128 n <= 5150, big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) -> seq_t */
#include "../utils/pow3.h"       /* pow3(idx_t) -> seq_t */

/* Cache for A006130 values. */
static seq_t *a_cache = NULL;
static unsigned char *a_computed = NULL;
static idx_t a_cache_cap = 0;

/* Ensure the cache can hold values [0..n]. */
static void ensure_cache(idx_t n)
{
    if (n < a_cache_cap)
        return;
    idx_t new_cap = n + 1;
    a_cache      = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_computed   = (unsigned char *)realloc(a_computed,
                                            new_cap * sizeof(unsigned char));
    if (!a_cache || !a_computed)
        exit(1);                       /* allocation failure */
    for (idx_t i = a_cache_cap; i < new_cap; ++i)
        a_computed[i] = 0;
    a_cache_cap = new_cap;
}

/* Compute A006130(n) if not already cached. */
static seq_t get_A006130(idx_t n)
{
    ensure_cache(n);
    if (a_computed[n])
        return a_cache[n];
    seq_t sum = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t bj = binomial(n - j, j);
        seq_t pj = pow3(j);
        sum += bj * pj;
    }
    a_cache[n] = sum;
    a_computed[n] = 1;
    return sum;
}

/* Find the largest r such that r(r+1)/2 ≤ n, using 128‑bit arithmetic. */
static idx_t triangular_row(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return get_A006130(0);   /* T(0,0) = 1 */

    idx_t r = triangular_row(n);
    idx_t prev_tri = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - prev_tri;

    seq_t val_k   = get_A006130(k);
    seq_t val_r   = get_A006130(r);
    seq_t val_rk  = get_A006130(r - k);

    return val_k - val_r + val_rk;
}
