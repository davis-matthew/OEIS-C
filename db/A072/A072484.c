/*
 * A072484  Triangle in which the n-th row contains the n smallest numbers
 *          containing the string of digits of n in increasing order.
 * https://oeis.org/A072484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 7811, u32 n <= 7811, u64 n <= 7811,
 *            u128 n <= 7811, big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A072484: the triangle whose n‑th row contains the n smallest
 * integers whose decimal representation contains the string of digits of n,
 * listed in increasing order.  The overall sequence a(n) is the row‑major
 * flattening of the triangle.  The function generates rows on demand and
 * caches them. */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system.  The helper functions
 * are available in the utils library.  We need seq_to_str to test whether
 * a number contains the required digit string. */
#include "../utils/seq_to_str.h"

static seq_t **row_cache   = NULL; /* array of pointers to each row */
static idx_t  cache_cap    = 0;    /* current capacity of row_cache   */
static idx_t  cache_rows   = 0;    /* number of rows that have been computed */

/* Generate the r‑th row (1‑based) and store it in row_cache[r‑1] */
static void compute_row(idx_t r)
{
    if (r == 0) return;

    /* Convert the row index r to its decimal string representation. */
    char pat[32];
    snprintf(pat, sizeof(pat), "%llu", (unsigned long long)r);

    /* Allocate storage for the r elements of this row. */
    seq_t *row = (seq_t *)malloc(r * sizeof(seq_t));
    if (!row) exit(1);                    /* allocation failure */

    idx_t  count = 0;
    seq_t  cur   = (seq_t)r;              /* start searching from r itself */

    /* Temporary buffer for converting seq_t to a string. */
    char buf[256];
    for (;; cur = cur + 1)
    {
        size_t len = 0;
        seq_to_str(cur, buf, &len);       /* conversion into buf */

        /* If the result is longer than buf, allocate a bigger buffer. */
        if (len >= sizeof(buf))
        {
            char *big = (char *)malloc(len + 1);
            if (!big) exit(1);
            seq_to_str(cur, big, &len);
            big[len] = '\0';
            if (strstr(big, pat))
                row[count++] = cur;
            free(big);
        }
        else
        {
            buf[len] = '\0';
            if (strstr(buf, pat))
                row[count++] = cur;
        }

        if (count == r) break;
    }

    row_cache[r - 1] = row;
}

/* Ensure that all rows up to 'need' have been computed and cached. */
static void ensure_rows(idx_t need)
{
    if (need <= cache_rows) return;

    if (need > cache_cap)
    {
        seq_t **new_cache = (seq_t **)realloc(row_cache,
                                             need * sizeof(seq_t *));
        if (!new_cache) exit(1);
        row_cache = new_cache;
        cache_cap = need;
    }

    for (idx_t r = cache_rows + 1; r <= need; ++r)
        compute_row(r);

    cache_rows = need;
}

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence is 1‑based */

    /* Find the row r such that the first r numbers of this row happen
     * after the previous rows.  We find the smallest r with r*(r+1)/2 >= n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Position within the row (1‑based). */
    idx_t prev_sum = (r * (r - 1)) / 2;        /* sum of first r-1 row sizes */
    idx_t k = n - prev_sum;                    /* column index */

    /* Make sure the needed rows are cached. */
    ensure_rows(r);

    return row_cache[r - 1][k - 1];
}
