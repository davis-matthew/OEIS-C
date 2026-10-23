/*
 * A134637  Triangle, T(n,k) = T(n-1,k) + T(n-1,k-1), 1<k<n with borders given
 *          by the Tetrahedral numbers.
 * https://oeis.org/A134637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattened T(n,k) of a triangle T(1,1) = 1
 *            T(n,1)=T(n,n)=n*(n+1)*(n+2)/6 (tetrahedral numbers)
 *            T(n,k)=T(n-1,k-1)+T(n-1,k) 1<k<n The sequence is obtained by
 *            listing rows left to right
 * Range:     u8 n <= 30, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t tetrahedral(idx_t n)
{
    seq_t a = (seq_t)n;
    return a * (a + 1) * (a + 2) / 6;
}

/* Return the nth term of sequence A134637 (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* not defined, safe fallback */

    /* Find the row r that contains position n. */
    idx_t r = 1;
    idx_t sum = 1;                      /* cumulative number of terms up to current row */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Zero‑based index within the row */
    idx_t pos = n - (sum - r) - 1;
    if (r == 1) return 1;               /* trivial first row */

    /* Build the triangle row by row up to row r. */
    seq_t *prev = (seq_t *)malloc((r - 1) * sizeof(seq_t));
    if (!prev) exit(1);                 /* allocation failure */
    prev[0] = 1;                        /* row 1 */

    for (idx_t cur = 2; cur <= r; ++cur) {
        seq_t *currow = (seq_t *)malloc(cur * sizeof(seq_t));
        if (!currow) { free(prev); exit(1); }

        /* Borders of the current row. */
        currow[0] = tetrahedral(cur);
        currow[cur - 1] = tetrahedral(cur);

        /* Interior entries using Pascal's rule. */
        for (idx_t k = 1; k + 1 < cur; ++k) {
            currow[k] = prev[k - 1] + prev[k];
        }

        free(prev);
        prev = currow;
    }

    seq_t result = prev[pos];
    free(prev);
    return result;
}
