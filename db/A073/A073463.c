/*
 * A073463  Triangle of number of partitions of 2n into powers of 2 where the
 *          largest part is 2^k.
 * https://oeis.org/A073463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where r is the unique integer with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 and c = n – r(r+1)/2. T(n,k) = T(n-1,k) +
 *            T([n/2],k-1) with T(n,0)=1 and T(0,k)=0 for k>0
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* table[ row ][ col ]  (col ≤ row) */
static seq_t **table = NULL;      /* pointers to rows */
static size_t  cap_rows = 0;      /* allocated row‑pointer capacity   */
static size_t  max_row  = 0;      /* largest row fully computed so far */

/* allocate and compute rows up to r (inclusive) */
static void
ensure_rows(size_t r)
{
    if (table == NULL) {          /* first call: initialise table */
        cap_rows = r + 1;
        table = (seq_t **)malloc(cap_rows * sizeof(seq_t *));
        for (size_t i = 0; i < cap_rows; ++i) table[i] = NULL;

        /* row 0 : T(0,0)=1 */
        table[0] = (seq_t *)malloc(sizeof(seq_t));
        table[0][0] = (seq_t)1;
        max_row = 0;
    }

    if (r <= max_row) return;     /* already computed */

    /* grow the array of row pointers if required */
    if (cap_rows <= r) {
        size_t new_cap = cap_rows ? cap_rows : 1;
        while (new_cap <= r) new_cap <<= 1;
        table = (seq_t **)realloc(table, new_cap * sizeof(seq_t *));
        for (size_t i = cap_rows; i < new_cap; ++i) table[i] = NULL;
        cap_rows = new_cap;
    }

    /* compute the new rows */
    for (size_t n = max_row + 1; n <= r; ++n) {
        table[n] = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        /* k = 0 */
        table[n][0] = (seq_t)1;
        /* other columns */
        for (size_t k = 1; k <= n; ++k) {
            seq_t a = (k <= n - 1) ? table[n - 1][k] : (seq_t)0;
            size_t half = n >> 1;
            seq_t b = (k - 1 <= half) ? table[half][k - 1] : (seq_t)0;
            table[n][k] = a + b;
        }
    }
    max_row = r;
}

/* Return a(n) for the sequence A073463. */
seq_t a(idx_t n)
{
    /* convert index to row and column of the triangle */
    size_t r = (size_t)((sqrt((double)8.0 * (double)n + 1.0) - 1.0) / 2.0);
    /* correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= (size_t)n) ++r;
    while (r * (r + 1) / 2 > (size_t)n) --r;

    size_t col = (size_t)(n - r * (r + 1) / 2);

    ensure_rows(r);
    return table[r][col];
}
