/*
 * A167884  Triangle read by rows: T(n,k) given by T(n, 1) = T(n,n) = 1,
 *          otherwise T(n, k) = (m*n-m*k+1)*T(n-1,k-1) + (m*k-m+1)*T(n-1,k),
 *          where m = 8.
 * https://oeis.org/A167884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) with T defined by T(1,1)=1; T(n,1)=T(n,n)=1;
 *            T(n,k) = (m*n - m*k + 1) * T(n-1,k-1) + (m*k - m + 1) * T(n-1,k)
 *            where m = 8. a(n) is the sequence obtained by reading this
 *            triangle row‑by‑row (rows and columns start at 1)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 39, u64 n <= 96, u128 n <= 263,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **rows = NULL;   /* rows[1] is row 1 (index 0 unused) */
static size_t rowsCap = 0;    /* capacity of rows pointer array   */
static idx_t  maxRow = 0;     /* highest computed row              */

/* ensure that rows up to r are computed */
static void ensure_rows(idx_t r)
{
    if (maxRow >= r)
        return;

    /* grow the rows pointer array if necessary */
    if (rowsCap < (size_t)(r + 1)) {
        size_t newCap = rowsCap ? rowsCap * 2 : 16;
        while (newCap < (size_t)(r + 1))
            newCap <<= 1;
        seq_t **tmp = (seq_t **)realloc(rows, newCap * sizeof(seq_t *));
        rows = tmp;
        rowsCap = newCap;
    }

    /* compute missing rows one by one */
    while (maxRow < r) {
        idx_t next = maxRow + 1;           /* row number (1‑based) */
        size_t len = (size_t)next;         /* length of the row   */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row)
            return; /* allocation failed */

        /* base values */
        row[0] = 1;
        if (len > 1)
            row[len - 1] = 1;

        /* interior entries */
        if (len > 2) {
            for (idx_t pos = 1; pos < next - 1; ++pos) { /* 0‑based pos  */
                /* pos corresponds to k = pos+1 (1‑based) */
                seq_t k = (seq_t)(pos + 1);
                seq_t a = ((seq_t)8 * (seq_t)next) - ((seq_t)8 * k) + 1;
                seq_t b = ((seq_t)8 * k) - 8 + 1;
                row[pos] = a * rows[maxRow][pos - 1] + b * rows[maxRow][pos];
            }
        }

        rows[next] = row;
        maxRow = next;
    }
}

seq_t a(idx_t n)
{
    /* locate the row r that contains position n (row‑major) */
    idx_t low = 1, high = n; /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((seq_t)mid * (seq_t)(mid + 1)) / 2 >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* column k (1‑based) within that row */
    idx_t prevTri = (idx_t)(((seq_t)(r - 1) * (seq_t)r) / 2);
    idx_t k = n - prevTri; /* 1‑based position in row r */

    ensure_rows(r);
    return rows[r][k - 1];
}
