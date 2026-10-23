/*
 * A120919  Cascadence of (1+x)^3; a triangle, read by rows of 3n+1 terms,
 *          that retains its original form upon convolving each row with
 *          [1,3,3,1] and then letting excess terms spill over from each row
 *          into the initial positions of the next row such that only 3n+1
 *          terms remain in row n for n>=0.
 * https://oeis.org/A120919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A120919
    Recurrence:  Build a triangle of 3n+1 terms.
    T[0] = {1}.  For n ≥ 1 let P be row n–1 of length L = 3(n–1)+1.
    Convolve P with the fixed kernel [1,3,3,1] producing a length L+3
    array S.  Row n is formed by taking S[1],S[2]…S[L+2] and finally
    S[1] again.  The flattened sequence a(n) is obtained by reading the
    triangle row‑by‑row (row 0 first, then row 1, etc.). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ----------  dynamic storage  ---------- */
static seq_t **rows = NULL;     /* pointers to each stored row */
static size_t *row_len   = NULL;/* length of each stored row   */
static size_t *row_start = NULL;/* starting index of each row   */
static size_t  rows_cap = 0;     /* current capacity of arrays  */
static size_t  rows_used = 0;    /* number of rows currently stored */

/* increase storage capacity if needed */
static void ensure_capacity(size_t need)
{
    if (need < rows_cap)
        return;
    size_t new_cap = rows_cap ? rows_cap << 1 : 16;
    while (new_cap <= need)
        new_cap <<= 1;

    seq_t **r  = (seq_t **)realloc(rows,      new_cap * sizeof(seq_t *));
    size_t *l  = (size_t *)realloc(row_len,   new_cap * sizeof(size_t));
    size_t *s  = (size_t *)realloc(row_start, new_cap * sizeof(size_t));
    if (!r || !l || !s)
        abort(); /* fatal allocation failure */
    rows      = r;
    row_len   = l;
    row_start = s;
    rows_cap  = new_cap;
}

/* create the first row {1} */
static void init_row0(void)
{
    if (rows_used == 0)
    {
        ensure_capacity(1);
        rows[0] = (seq_t *)malloc(sizeof(seq_t));
        rows[0][0] = 1;
        row_len[0]   = 1;
        row_start[0] = 0;
        rows_used = 1;
    }
}

/* compute row r (r ≥ 1).  Assumes row r‑1 has already been computed. */
static void compute_row(size_t r)
{
    size_t prev_len = row_len[r - 1];     /* length of previous row   */
    size_t new_len  = prev_len + 3;      /* length of the new row    */
    seq_t *prev = rows[r - 1];

    /* convolution: S[i] = Σ_{k=0..3} prev[i-k] * coeff[k]  (if index valid) */
    const seq_t coeff[4] = {1, 3, 3, 1};
    seq_t *conv = (seq_t *)malloc(new_len * sizeof(seq_t));
    for (size_t i = 0; i < new_len; ++i)
    {
        seq_t val = 0;
        for (size_t k = 0; k < 4; ++k)
        {
            if (i >= k)
            {
                size_t j = i - k;
                if (j < prev_len)
                    val += prev[j] * coeff[k];
            }
        }
        conv[i] = val;
    }

    /* build the new row: S[1]…S[new_len-1] then S[1] again */
    seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));
    for (size_t i = 0; i < new_len - 1; ++i)
        new_row[i] = conv[i + 1];
    new_row[new_len - 1] = conv[1];

    free(conv);

    /* store the new row */
    ensure_capacity(r);
    rows[r] = new_row;
    row_len[r]   = new_len;
    row_start[r] = row_start[r - 1] + row_len[r - 1];
    if (rows_used <= r)
        rows_used = r + 1;
}

/* make sure that the row containing index n has been computed */
static void ensure_rows_up_to(idx_t n)
{
    init_row0();
    while ((size_t)n >= row_start[rows_used - 1] + row_len[rows_used - 1])
        compute_row(rows_used);
}

/* public interface */
seq_t a(idx_t n)
{
    ensure_rows_up_to(n);
    size_t r   = rows_used - 1;                 /* row that contains n   */
    size_t pos = (size_t)(n - row_start[r]);   /* position inside that row */
    return rows[r][pos];
}
