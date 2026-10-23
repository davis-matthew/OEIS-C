/*
 * A188066  Triangle read by rows: Bell polynomial of the second kind B(n,k)
 *          with argument vector (7, 42, 210, 840, 2520, 5040, 5040).
 * https://oeis.org/A188066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = B(r,k) where r and k are chosen by reading a triangular
 *            array row‑by‑row. The triangle entries are defined by the Bell
 *            polynomial of the second kind: B(n,k) = (n!/k!) * Σ_{j=0..k}
 *            C(k,j) * C(7*j,n) * (-1)^(k-j) The index n with offset 1 is
 *            converted to the pair (r,k) by solving T(r-1) < n...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"  /* seq_t binomial(idx_t n, idx_t k); */

static seq_t *fact_cache = NULL;
static idx_t fact_cache_size = 0;

/* Ensure factorial values up to n are cached in fact_cache[*].  */
static void
ensure_fact_cache(idx_t n)
{
    if (n < fact_cache_size) return;
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    /* Compute missing factorials. */
    for (idx_t i = fact_cache_size; i <= n; ++i) {
        if (i == 0) new_cache[0] = 1;
        else new_cache[i] = new_cache[i - 1] * (seq_t)i;
    }
    fact_cache = new_cache;
    fact_cache_size = new_size;
}

/* Compute B(n,k) according to the formula.  Assumes 1 ≤ k ≤ n. */
static seq_t
B_of(idx_t n, idx_t k)
{
    if (k == 0 || k > n) return 0;

    ensure_fact_cache(n);
    seq_t ratio = fact_cache[n] / fact_cache[k];

    seq_t sum = 0;
    /* Binomial(7*j, n) is zero for 7*j < n, so start at ceil(n/7). */
    idx_t j_start = (n + 6) / 7;          /* ceil(n/7) */
    if (j_start > k) return 0;           /* empty sum */

    for (idx_t j = j_start; j <= k; ++j) {
        seq_t bin1 = binomial(k, j);
        seq_t bin2 = binomial(7 * j, n);
        seq_t term = bin1 * bin2;
        if ((k - j) & 1u)  /* odd exponent → negative */
            sum -= term;
        else
            sum += term;
    }
    return ratio * sum;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1, but guard against zero */
    /* Find row r such that T(r-1) < n ≤ T(r). */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tmid = mid * (mid + 1) / 2;
        if (tmid >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t prev_total = r * (r - 1) / 2;
    idx_t pos_in_row = n - prev_total;   /* 1‑based position in row r */
    idx_t k = pos_in_row;                /* column index in the triangle */
    return B_of(r, k);
}
