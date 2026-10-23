/*
 * A125233  Triangle T(n,k) read by rows, the (n-k)-th term of the k times
 *          repeated partial sum of the hexagonal numbers, 0 <= k < n, 0 < n.
 * https://oeis.org/A125233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) defined by:
   T(n,0) = A000384(n+1) = (n+1)*(2*(n+1)-1)
   T(n,k) = T(n-1,k) + T(n-1,k-1)   for 1 ≤ k < n
   T(n,n) = T(n-1,n-1)              (border)
   a(n) is the flattening of the triangle row‑by‑row. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *data = NULL;      /* flattened triangle data */
static size_t dataCap  = 0;     /* capacity of data array */
static size_t dataSize = 0;     /* number of elements stored */
static idx_t  lastRow  = (idx_t)-1;  /* highest row computed (-1 initially) */

/* Ensure capacity for at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= dataCap)
        return;
    size_t newCap = dataCap ? dataCap * 2 : 8;
    while (newCap < needed)
        newCap *= 2;
    seq_t *newData = (seq_t *)realloc(data, newCap * sizeof(seq_t));
    if (!newData) exit(1);
    data    = newData;
    dataCap = newCap;
}

/* Compute rows up to and including 'row', extending the cache. */
static void compute_rows_upto(idx_t row)
{
    /* If no rows yet, start from row 0. */
    if (lastRow == (idx_t)-1) lastRow = 0;
    for (idx_t r = lastRow; r <= row; ++r)
    {
        idx_t startCur  = r * (r + 1) / 2;
        idx_t len       = r + 1;
        idx_t startPrev = (r > 0) ? (r - 1) * r / 2 : 0;

        /* Make space for the new row. */
        ensure_capacity(startCur + len);

        /* k = 0 : left border (hexagonal number). */
        seq_t hex = (seq_t)(r + 1) * ((seq_t)(2 * (r + 1)) - 1);
        data[startCur] = hex;

        if (r > 0)
        {
            /* k = 1 .. r-1 : interior values. */
            for (idx_t k = 1; k < r; ++k)
                data[startCur + k] = data[startPrev + k] + data[startPrev + k - 1];

            /* k = r : right border. */
            data[startCur + r] = data[startPrev + r - 1];
        }

        dataSize = startCur + len;
        lastRow  = r;
    }
}

seq_t a(idx_t n)
{
    /* Determine which row contains index n. */
    idx_t row = 0;
    while ((row + 1) * (row + 2) / 2 <= n)
        ++row;

    /* Ensure cache has all rows up to 'row'. */
    if (dataSize == 0 || row > lastRow)
        compute_rows_upto(row);

    /* Retrieve the requested element. */
    idx_t startRow = row * (row + 1) / 2;
    idx_t col      = n - startRow;
    return data[startRow + col];
}
