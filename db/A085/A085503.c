/*
 * A085503  Sub-triangle of A008292: take every second term of every second
 *          row.
 * https://oeis.org/A085503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Eulerian(2r+1, 2c) where r = floor( (sqrt(8n+1)-1)/2 )
 *            and c = n - r(r+1)/2. The Eulerian numbers are defined by
 *            A(0,0)=1 A(n,k) = (n-k)*A(n-1,k-1) + (k+1)*A(n-1,k) for n>=1,
 *            0<=k<=n
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **eulerian_cache = NULL;   /* eulerian_cache[i][k] = A(i,k) */
static size_t cached_rows = 0;          /* number of rows currently cached (incl. row 0) */

/* extend the cache to contain rows 0..need (inclusive) */
static void compute_eulerian_up_to(size_t need)
{
    if (need < cached_rows)
        return;
    eulerian_cache = (seq_t **)realloc(eulerian_cache,
                                       (need + 1) * sizeof(seq_t *));
    for (size_t i = cached_rows; i <= need; ++i) {
        eulerian_cache[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (i == 0) {
            eulerian_cache[0][0] = (seq_t)1;
        } else {
            for (size_t k = 0; k <= i; ++k) {
                seq_t val = 0;
                if (k > 0)
                    val += (seq_t)(i - k) * eulerian_cache[i - 1][k - 1];
                if (k <= i - 1)
                    val += (seq_t)(k + 1) * eulerian_cache[i - 1][k];
                eulerian_cache[i][k] = val;
            }
        }
    }
    cached_rows = need + 1;
}

seq_t a(idx_t n)
{
    /* compute r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;           /* 0 <= c <= r */

    idx_t rowIdx = 2 * r + 1;       /* Eulerian row index */
    idx_t colIdx = 2 * c;           /* Eulerian column index */

    compute_eulerian_up_to((size_t)rowIdx);
    return eulerian_cache[rowIdx][colIdx];
}
