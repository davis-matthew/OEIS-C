/*
 * A187115  Triangle, read by rows, where row n equals the coefficients of y^k
 *          in R_{n-1}(y+y^2) for k=2..n, where R_n(y) is the n-th row
 *          polynomial in y for n>=2 with R_2(y)=y^2.
 * https://oeis.org/A187115
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of y^k in the (n-2)-th row of the triangle T
 *            with T(2,2)=1 and T(n,k) = Σ_{j=⌈k/2⌉}^{min(k,n-1)} C(j,k-j) *
 *            T(n-1,j) for 2 ≤ k ≤ n (n ≥ 2). Sequence a(n) is the row‑by‑row
 *            flattening of T, with offset 2
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial, offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/offset_row.h"   /* returns seq_t */
#include "../utils/binomial.h"    /* returns seq_t */

/* Static cache for computed values of the sequence. */
static seq_t *data      = NULL;   /* flattened triangle, linear index 0 ↔ a(2) */
static size_t data_cap  = 0;      /* allocated capacity */
static size_t data_len  = 0;      /* number of values currently stored */
static idx_t  max_row   = 1;      /* largest n for which a(n) is computed (row 1 unused) */
/* --------------------------------------------------------------------- */
static void ensure_capacity(size_t needed)
{
    if (data_cap < needed)
    {
        size_t new_cap = data_cap ? (data_cap << 1) : 32;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        data = tmp;
        data_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */

/* Compute a(n) for n ≥ 2.
   The cache is updated incrementally; calls may come in any order. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below the offset are irrelevant */

    /* Linear index of a(n) in the flattened array: first term a(2)
       corresponds to index 0. */
    const size_t target = (size_t)(n - 2);

    /* Initialise cache on first use. */
    if (!data)
    {
        ensure_capacity(32);
        data_len = 0;
        max_row  = 1;              /* no rows computed yet */
    }

    /* Bring the cache up to the requested term if necessary. */
    while (data_len <= target)
    {
        /* Compute the next row of the triangle.  Row numbers start at 2. */
        const idx_t row = ++max_row;            /* row to be computed */

        /* Length of this row: one element per k = 2..row. */
        const idx_t row_len = row - 1;

        /* Ensure there is room for the new row. */
        ensure_capacity(data_len + (size_t)row_len);

        /* Linear index where this row begins – it equals data_len. */
        const size_t start = data_len;
        const size_t prev_start = start - (size_t)(row - 2); /* start of row-1 */

        /* T(row,2) = 1 */
        data[start] = 1;

        /* Compute remaining entries of the row. */
        for (idx_t k = 3; k <= row; ++k)
        {
            seq_t sum = 0;   /* sum over j */

            /* j runs from ceil(k/2) to min(k, row-1).  Note row-1 ≤ n-1 always. */
            const idx_t j_start = (k + 1) >> 1;          /* ceil(k/2) */
            const idx_t j_end   = (k < row) ? k : row - 1;

            for (idx_t j = j_start; j <= j_end; ++j)
            {
                seq_t binom = binomial(j, k - j);
                seq_t prev  = data[prev_start + (size_t)(j - 2)];
                sum += (binom * prev);   /* operator+ and * are defined for seq_t */
            }

            /* Store the computed coefficient.  The offset within the row is k-2. */
            data[start + (size_t)(k - 2)] = sum;
        }

        /* Update cache state. */
        data_len += (size_t)row_len;
    }

    return data[target];
}
