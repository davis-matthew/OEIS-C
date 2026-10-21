/*
 * A064884  Eisenstein array Ei(3,1).
 * https://oeis.org/A064884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the flattened row‑major order of the Eisenstein * array
 *            Ei(3,1). The array a(r,m) is defined by * a(1,0)=3, a(1,1)=1 *
 *            for r>1: * if m is even: a(r,m)=a(r‑1,m/2) * if m is odd :
 *            a(r,m)=a(r‑1,(m‑1)/2)+a(r‑1,(m+1)/2) * The sequence a(n) outputs
 *            the values row by row: * row 1: 3,1 * row 2: 3...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* Helper wrappers are not needed, but the following %product% type
 * seq_t and idx_t are supplied by the build environment.
 */

/* static state for memoization */
static seq_t **rows = NULL;      /* pointers to rows r=1..R   */
static size_t *rowlens = NULL;   /* lengths of each row        */
static size_t maxRows = 0;       /* number of rows computed    */
static size_t totalCount = 0;    /* total number of stored terms */

/* ensure we have precomputed rows until the cumulative count
 * reaches at least target (i.e. covers index n>=target)            */
static void ensure_precomputed(idx_t target)
{
    while (totalCount < target) {
        /* new row number (1‑based) */
        size_t r = maxRows + 1;

        /* length = 2^(r-1)+1 */
        size_t len = (size_t)1 << (r - 1);
        len += 1;

        /* allocate memory for this row */
        seq_t *newRow = (seq_t*)malloc(len * sizeof(seq_t));
        assert(newRow != NULL);

        if (r == 1) {
            newRow[0] = 3;
            newRow[1] = 1;
        } else {
            seq_t *prev = rows[maxRows - 1];  /* previous row (r‑1) */
            for (size_t m = 0; m < len; ++m) {
                if ((m & 1) == 0) {
                    newRow[m] = prev[m / 2];
                } else {
                    newRow[m] = prev[(m - 1) / 2] + prev[(m + 1) / 2];
                }
            }
        }

        /* grow arrays of pointers/lens */
        rows = (seq_t**)realloc(rows, sizeof(seq_t*) * (maxRows + 1));
        rowlens = (size_t*)realloc(rowlens, sizeof(size_t) * (maxRows + 1));
        assert(rows != NULL && rowlens != NULL);

        rows[maxRows] = newRow;
        rowlens[maxRows] = len;

        totalCount += len;
        maxRows += 1;
    }
}

/* public interface: return the n‑th term of A064884 (1‑based) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined region, return 0 */
    ensure_precomputed((size_t)n);

    /* locate the row that contains the n‑th term */
    size_t idx = (size_t)(n - 1);  /* convert to 0‑based index */
    size_t cum = 0;
    for (size_t r = 0; r < maxRows; ++r) {
        if (cum + rowlens[r] > idx) {
            size_t col = idx - cum;
            return rows[r][col];
        }
        cum += rowlens[r];
    }

    /* control should never reach here because ensure_precomputed
     * guarantees enough terms. */
    return (seq_t)0;
}
