/*
 * A027186  Triangular array E by rows: E(n,k) = number of partitions of n
 *          into an even number of parts, the least being k.
 * https://oeis.org/A027186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = E(r,c) where E(n,k) = number of partitions of n into an
 *            even number of parts with the least part k. The triangular array
 *            E is built together with its companion O (odd parts) by: E(n,k)
 *            = Σ_{j=k}^{n-k} O(n-k,j) O(n,k) = Σ_{j=k}^{n-k} E(n-k,j) with
 *            base O(k,k)=1 and E(k,k)=0
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *E_cache = NULL;
static seq_t *O_cache = NULL;
static idx_t max_cached = 0;

/* Rebuild the triangular tables up to the specified row (inclusive). */
static void rebuild_tables(idx_t max_row)
{
    if (E_cache) free(E_cache);
    if (O_cache) free(O_cache);

    size_t width = (size_t)(max_row + 1);          /* number of columns per row (includes column 0) */
    size_t total = width * width;                  /* enough space for all rows and columns up to max_row */

    E_cache = (seq_t *)calloc(total, sizeof(seq_t));
    O_cache = (seq_t *)calloc(total, sizeof(seq_t));
    if (!E_cache || !O_cache) return;              /* out of memory – unlikely with small max_row */

    for (idx_t n = 1; n <= max_row; ++n)
    {
        for (idx_t k = 1; k <= n; ++k)
        {
            size_t idx = (size_t)n * width + (size_t)k;

            if (k == n) {                          /* base case */
                O_cache[idx] = (seq_t)1;
                E_cache[idx] = (seq_t)0;
                continue;
            }

            idx_t limit = n - k;                   /* upper bound for the sum */
            seq_t sumO = 0;
            seq_t sumE = 0;
            if (limit >= k)
            {
                size_t row_base = (size_t)limit * width;
                for (idx_t j = k; j <= limit; ++j)
                {
                    sumO += O_cache[row_base + (size_t)j];
                    sumE += E_cache[row_base + (size_t)j];
                }
            }
            E_cache[idx] = sumO;
            O_cache[idx] = sumE;
        }
    }

    max_cached = max_row;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* out of range, defined arbitrarily */

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    /* Compute column k (1‑based) within the row. */
    idx_t prev_total = (r - 1) * r / 2;
    idx_t k = n - prev_total;

    if (r > max_cached)
        rebuild_tables(r);

    size_t width = (size_t)(max_cached + 1);
    size_t idx = (size_t)r * width + (size_t)k;
    return E_cache[idx];
}
