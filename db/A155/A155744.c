/*
 * A155744  Triangle T(n, k) = (-1)^(n-k)*StirlingS1(n, k) +
 *          (-1)^k*StirlingS1(n, n-k) + (-1)^n*StirlingS1(n, k)*StirlingS1(n,
 *          n-k), read by rows.
 * https://oeis.org/A155744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (-1)^(n-k)*StirlingS1(n,k) + (-1)^k*StirlingS1(n,n-k) +
 *            (-1)^n*StirlingS1(n,k)*StirlingS1(n,n-k) The sequence a(n) is
 *            the triangle read by rows
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 57, u64 n <= 125,
 *            u128 n <= 334, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of rows of unsigned Stirling numbers of the first kind. */
static seq_t **cached_rows = NULL;
static idx_t max_cached = 0;

/* Ensure that rows 0..n are cached. */
static void ensure_rows(idx_t n)
{
    if (n < max_cached)
        return;

    /* Grow the pointer array. */
    if (max_cached == 0)
        cached_rows = (seq_t**)calloc(n + 1, sizeof(seq_t*));
    else
        cached_rows = (seq_t**)realloc(cached_rows, (n + 1) * sizeof(seq_t*));

    /* Generate rows from the first missing one up to n. */
    for (idx_t i = max_cached; i <= n; ++i)
    {
        seq_t *row = (seq_t*)calloc(i + 1, sizeof(seq_t));
        if (i == 0)
        {
            row[0] = (seq_t)1;
        }
        else
        {
            seq_t *prev = cached_rows[i - 1];

            /* Inductively fill the row (Stirling numbers of the first kind) */
            for (idx_t k = 1; k < i; ++k)
            {
                seq_t mult = (seq_t)(i - 1) * prev[k];
                row[k] = prev[k - 1] + mult;
            }
            row[i] = (seq_t)1;   /* c(i,i)=1 */
        }
        cached_rows[i] = row;
    }
    max_cached = n + 1;
}

/* Binary search to find the row that contains the linear index n. */
static idx_t row_of(idx_t n)
{
    /* The largest n for which n*(n+1)/2 <= n is n itself if n is small.
       We use an exclusive upper bound high = n+1 which will not overflow
       for the typical sizes used in tests. */
    idx_t low = 0;
    idx_t high = n + 1;          /* high is exclusive */

    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Find row r and column c for the linear index n. */
    idx_t r = row_of(n);
    __int128 offset128 = (__int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)offset128;
    idx_t c = n - offset;

    /* Make sure the required row is cached. */
    ensure_rows(r);

    seq_t val = cached_rows[r][c];
    seq_t rev = cached_rows[r][r - c];
    seq_t prod = val * rev;
    seq_t result = val + rev + prod;
    return result;
}
