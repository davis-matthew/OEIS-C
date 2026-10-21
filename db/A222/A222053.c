/*
 * A222053  O.g.f.: Sum_{n>=0} (n^3*x)^n/(1-n^3*x)^n * exp(-n^3*x/(1-n^3*x)) /
 *          n!.
 * https://oeis.org/A222053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} C(n-1,k-1) * S2(3*n,k) for n>0 with a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache for a(n) values */
static seq_t *a_cache = NULL;
static size_t cache_cap = 0;

/* Current row of Stirling numbers S2(m,k) (k from 0..m) */
static seq_t *row = NULL;
static size_t row_cap = 0;

/* Largest n that has been cached and the current Stirling height (m = 3*n) */
static idx_t current_n = 0;
static idx_t current_m = 0; /* currently 3 * current_n */

/* Ensure the a_cache array can hold indices up to n */
static void
ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap > n)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while ((idx_t)new_cap <= n)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(EXIT_FAILURE);
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_ptr[i] = 0;
    a_cache = new_ptr;
    cache_cap = new_cap;
}

/* Ensure the row array can hold indices up to n */
static void
ensure_row_cap(idx_t n)
{
    if ((idx_t)row_cap > n)
        return;
    size_t new_cap = row_cap ? row_cap << 1 : 8;
    while ((idx_t)new_cap <= n)
        new_cap <<= 1;
    seq_t *new_row = (seq_t *)realloc(row, new_cap * sizeof(seq_t));
    if (!new_row)
        exit(EXIT_FAILURE);
    for (size_t i = row_cap; i < new_cap; ++i)
        new_row[i] = 0;
    row = new_row;
    row_cap = new_cap;
}

/* Compute the n-th term of OEIS sequence A222053 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Initialise caches on first call */
    if (current_n == 0) {
        ensure_cache(0);
        a_cache[0] = (seq_t)1;
        ensure_row_cap(0);
        row[0] = (seq_t)1;
        current_m = 0;
    }

    /* Return cached value if already known */
    if (n <= current_n)
        return a_cache[n];

    /* Compute values for all n up to the requested one */
    for (idx_t target = current_n + 1; target <= n; ++target) {
        /* Grow Stirling row to height 3 * target */
        idx_t new_m   = target * (idx_t)3;
        idx_t delta_m = new_m - current_m;
        for (idx_t step = 0; step < delta_m; ++step) {
            idx_t m = current_m + 1;
            ensure_row_cap(m);
            /* Update row: S2(m,k) = k * S2(m-1,k) + S2(m-1,k-1) */
            for (idx_t k = m; k >= 1; --k) {
                row[k] = (seq_t)k * row[k] + row[k - 1];
            }
            row[0] = (seq_t)0;
            current_m = m;
        }

        /* Compute a(target) from row[1..target] */
        seq_t sum   = 0;
        seq_t coeff = 1; /* C(target-1,0) */
        for (idx_t k = 1; k <= target; ++k) {
            sum += coeff * row[k];
            if (k != target) {
                /* coeff = coeff * (target - k) / k  (updates to C(target-1, k)) */
                coeff = coeff * (seq_t)(target - k) / (seq_t)k;
            }
        }
        ensure_cache(target);
        a_cache[target] = sum;
    }

    current_n = n;
    return a_cache[n];
}
