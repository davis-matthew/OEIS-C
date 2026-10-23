/*
 * A111377  Triangle of sum of product of partitions of n where largest part
 *          is k.
 * https://oeis.org/A111377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle T(n,k) for n≥0, 0≤k≤n : T(0,0)=1, T(n,0)=0 for n>0,
 *            T(n,n)=n for n≥1, T(n,k)=k * Σ_{j=0}^{min(k,n−k)} T(n−k,j) (1)
 *            The sequence a(n) is obtained by reading the triangle row‑by‑row
 *            (left to right). The function below returns the n‑th such
 *            element
 * Range:     u8 n <= 58, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **TR = NULL;
static size_t nrows = 0;      /* greatest row computed so far */
static size_t cap_rows = 0;   /* capacity of TR */

static void ensure_rows(size_t up_to)
{
    if (up_to <= nrows) return;

    /* grow the pointer array if needed */
    if (up_to + 1 > cap_rows) {
        size_t newcap = (cap_rows == 0) ? 1 : cap_rows * 2;
        if (newcap < up_to + 1) newcap = up_to + 1;
        seq_t **newptr = (seq_t **)malloc(newcap * sizeof(seq_t *));
        if (cap_rows > 0) {
            for (size_t i = 0; i < cap_rows; ++i) newptr[i] = TR[i];
            free(TR);
        }
        TR = newptr;
        cap_rows = newcap;
    }

    /* build rows up to 'up_to' */
    for (size_t r = nrows + 1; r <= up_to; ++r) {
        seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        row[0] = (seq_t)0;      /* T(r,0)=0 for r>0 */
        row[r] = (seq_t)r;      /* T(r,r)=r */

        for (size_t k = 1; k < r; ++k) {
            size_t m = r - k;                     /* row index of the part to sum */
            size_t maxj = (m < k) ? m : k;        /* j runs 0..maxj */
            seq_t sum = (seq_t)0;
            for (size_t j = 0; j <= maxj; ++j) {
                sum += TR[m][j];
            }
            row[k] = (seq_t)k * sum;
        }
        TR[r] = row;
    }
    nrows = up_to;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, but safe return */

    /* find row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    idx_t cum = 1;      /* cumulative count up to current row */
    while (cum < n) {
        ++r;
        cum += r;
    }

    idx_t prev = (r - 1) * r / 2;   /* entries before row r */
    idx_t k = n - prev;             /* column within row r (1‑based) */

    ensure_rows((size_t)r);

    return TR[(size_t)r][(size_t)k];
}
