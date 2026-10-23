/*
 * A152655  A vector recursion sequence: k = -3; m = 3; l = -3;
 *          a(n)=k*{0,a(n-2),0}+m*{-(m-1)/m,a(n-1)}++m*{a(n-1),-(m-1)/m}+l*{0,0,a(n-4),0,0}.
 * https://oeis.org/A152655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) = vector recursion sequence A152655 a(n) is the n-th
 *            element in the concatenation of rows of the triangle where each
 *            row r (0‑based) has r+1 elements. The triangle is defined
 *            recursively by the vector operations in the OEIS entry
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t **rows = NULL;          /* array of pointers to each row */
static size_t rows_alloc = 0;        /* allocated capacity of `rows` */
static size_t rows_count = 0;        /* number of rows currently computed */

/* Ensure that rows[0..r] all exist.  Computes missing rows in order. */
static void ensure_rows(size_t r)
{
    if (r >= rows_alloc) {
        size_t new_alloc = r + 1;
        seq_t **tmp = (seq_t**)realloc(rows, new_alloc * sizeof(seq_t*));
        rows = tmp;
        rows_alloc = new_alloc;
    }
    for (size_t cur = rows_count; cur <= r; ++cur) {
        seq_t *row = (seq_t*)malloc((cur + 1) * sizeof(seq_t));
        /* Base rows:   r = 0..3  */
        if (cur == 0) {
            row[0] = (seq_t)1;
        } else if (cur == 1) {
            row[0] = (seq_t)1;   row[1] = (seq_t)1;
        } else if (cur == 2) {
            row[0] = (seq_t)1;   row[1] = (seq_t)3;   row[2] = (seq_t)1;
        } else if (cur == 3) {
            row[0] = (seq_t)1;   row[1] = (seq_t)9;   row[2] = (seq_t)9;   row[3] = (seq_t)1;
        } else {                /*  cur >= 4 :  vector‑recursion part */
            seq_t *prev1 = rows[cur - 1];
            seq_t *prev2 = rows[cur - 2];
            seq_t *prev4 = rows[cur - 4];
            const seq_t CONST2 = (seq_t)2;   /* (m-1) with m = 3 */
            for (size_t i = 0; i <= cur; ++i) {
                seq_t posSum = 0;
                /* Positive contributions from previous row (cur‑1) */
                if (i > 0)        posSum += ((seq_t)3) * prev1[i - 1];
                if (i < cur)      posSum += ((seq_t)3) * prev1[i];
                seq_t val = posSum;
                /* Negative contributions */
                if (i > 0 && i < cur)                     val -= ((seq_t)3) * prev2[i - 1];
                if (i == 0)                                val -= CONST2;
                if (i == cur)                              val -= CONST2;
                if (i >= 2 && i <= cur - 2)                val -= ((seq_t)3) * prev4[i - 2];
                row[i] = val;
            }
        }
        rows[cur] = row;
        rows_count = cur + 1;
    }
}

/* Return the n-th term (0‑based) in the concatenated sequence. */
seq_t a(idx_t n)
{
    /* Find the row r such that n lies in row r */
    size_t r = 0;
    while (((size_t)r + 1) * ((size_t)r + 2) / 2 <= (size_t)n) {
        ++r;
    }
    /* Position inside row r */
    size_t pos = (size_t)n - r * (r + 1) / 2;
    /* Make sure rows up to r are computed */
    ensure_rows(r);
    return rows[r][pos];
}
