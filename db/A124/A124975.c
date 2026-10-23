/*
 * A124975  Table (read by antidiagonals): t(1,n) = t(m,1) = 1 for all m and
 *          n. t(m,n) = (product{k=1 to m-1} t(k,n)) + (product{k=1 to n-1}
 *          t(m,k)).
 * https://oeis.org/A124975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   t(1,n)=t(m,1)=1; t(m,n)= (∏_{k=1}^{m-1} t(k,n)) + (∏_{k=1}^{n-1}
 *            t(m,k)); a(n)=t(m,n) read by antidiagonals (offset 1)
 * Range:     u8 n <= 22, u16 n <= 29, u32 n <= 37, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required by the framework */
seq_t a(idx_t n);

#include <stdlib.h>

/* Internal static table holding t(m,n).  table[i][j] is valid
   for 1 ≤ i,j ≤ cur_d+1.  table is NULL until first use. */
static seq_t **table = NULL;
static idx_t cur_d = (idx_t)-1;   /* maximum diagonal computed so far */

/* Ensure that the table contains rows and columns up to diagonal d_target
   (i.e. indices 1 … d_target+1). */
static void ensure_table(idx_t d_target)
{
    if (table == NULL) {
        /* First allocation: create table of size (d_target+2) × (d_target+2)
           (indices 0..d_target+1).  Index 0 is unused. */
        idx_t new_dim = d_target + 1;
        table = (seq_t **)calloc((size_t)(new_dim + 1), sizeof(seq_t *));
        for (idx_t i = 1; i <= new_dim; ++i) {
            table[i] = (seq_t *)calloc((size_t)(new_dim + 1), sizeof(seq_t));
        }

        /* Compute all cells in this first block. */
        for (idx_t i = 1; i <= new_dim; ++i) {
            for (idx_t j = 1; j <= new_dim; ++j) {
                if (i == 1 || j == 1) {
                    table[i][j] = 1;
                } else {
                    seq_t prod1 = 1;
                    for (idx_t k = 1; k < i; ++k) prod1 *= table[k][j];
                    seq_t prod2 = 1;
                    for (idx_t k = 1; k < j; ++k) prod2 *= table[i][k];
                    table[i][j] = prod1 + prod2;
                }
            }
        }
        cur_d = d_target;
        return;
    }

    /* Table already allocated; extend only if needed. */
    if (d_target <= cur_d) return;

    idx_t old_dim = cur_d + 1;
    idx_t new_dim = d_target + 1;

    /* Resize the outer pointer array. */
    seq_t **tmp = (seq_t **)realloc(table, (size_t)(new_dim + 1) * sizeof(seq_t *));
    table = tmp;

    /* Create new rows as necessary. */
    for (idx_t i = old_dim + 1; i <= new_dim; ++i) {
        table[i] = (seq_t *)calloc((size_t)(new_dim + 1), sizeof(seq_t));
    }

    /* Enlarge existing rows and zero the new entries. */
    for (idx_t i = 1; i <= old_dim; ++i) {
        seq_t *row = (seq_t *)realloc(table[i], (size_t)(new_dim + 1) * sizeof(seq_t));
        table[i] = row;
        for (idx_t j = old_dim + 1; j <= new_dim; ++j) {
            row[j] = 0;
        }
    }

    /* Compute all cells that are still zero. */
    for (idx_t i = 1; i <= new_dim; ++i) {
        for (idx_t j = 1; j <= new_dim; ++j) {
            if (table[i][j] != 0) continue;
            if (i == 1 || j == 1) {
                table[i][j] = 1;
            } else {
                seq_t prod1 = 1;
                for (idx_t k = 1; k < i; ++k) prod1 *= table[k][j];
                seq_t prod2 = 1;
                for (idx_t k = 1; k < j; ++k) prod2 *= table[i][k];
                table[i][j] = prod1 + prod2;
            }
        }
    }

    cur_d = d_target;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* Determine the diagonal d containing the nth term.
       Number of terms up to diagonal d is (d+1)(d+2)/2. */
    idx_t d = 0;
    idx_t terms = 1;   /* for d = 0 */
    while (n > terms) {
        ++d;
        terms += d + 1;   /* add size of next diagonal */
    }

    /* Index of first term on diagonal d (1‑based). */
    idx_t start = (d * (d + 1ull) >> 1) + 1;
    idx_t offset = n - start;   /* 0‑based position within the diagonal */

    /* Coordinates of the desired table entry:
       first on diagonal → (d+1,1), last → (1,d+1). */
    idx_t m = d + 1 - offset;   /* row */
    idx_t col = offset + 1;     /* column */

    ensure_table(d);   /* make sure the table is big enough */

    return table[m][col];
}
