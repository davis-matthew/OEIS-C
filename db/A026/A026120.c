/*
 * A026120  Triangle T by rows: second differences of Motzkin triangle
 *          (A026300), (i >= -1, -1<=j<=i).
 * https://oeis.org/A026120
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A026120 – flattened Motzkin triangle.
 * T(i,0)=1, T(0,1)=0, T(1,1)=1, T(1,2)=0.
 * For i>=2:
 *   T(i,1)=i-1,
 *   T(i,j)=T(i-1,j-2)+T(i-1,j-1)+T(i-1,j)  (2<=j<=i),
 *   T(i,i+1)=T(i-1,i-1)+T(i-1,i).
 * The sequence consists of all entries of T read by rows.
 */

#include <stdlib.h>
#include <stddef.h>

/* external types supplied by the build environment */
extern seq_t a(idx_t n);   /* declaration for compiler consistency */

/* static state used by the function */
static seq_t *data      = NULL; /* flattened values of the triangle          */
static size_t data_len  = 0;    /* number of values stored                   */
static size_t data_cap  = 0;    /* capacity of the data array                 */

static seq_t *prev_row  = NULL; /* previous row of the triangle (as array)     */
static size_t prev_len  = 0;    /* length of the previous row                  */

static int last_row     = -2;   /* index of the last computed row (-2 means none) */

/* ensure that the data array can hold at least min_cap elements */
static void ensure_data_capacity(size_t min_cap)
{
    if (min_cap <= data_cap)
        return;
    size_t new_cap = data_cap ? data_cap : 64;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failure */
    data = tmp;
    data_cap = new_cap;
}

/* compute A026120 */
seq_t a(idx_t n)
{
    if (n < -1)                     /* below the defined offset */
        return (seq_t)0;

    size_t target = (size_t)(n + 1); /* index in the flattened array */

    /* generate rows until the desired index is available */
    while (data_len <= target)
    {
        int next_row = last_row + 1;           /* compute row i = next_row */
        size_t row_len = (size_t)(next_row + 2); /* length of the row */

        seq_t *new_row = (seq_t *)malloc(row_len * sizeof(seq_t));
        if (!new_row)
            abort();

        /* ----- construct the current row ----- */
        if (next_row == -1) {
            new_row[0] = (seq_t)1;
        } else if (next_row == 0) {
            new_row[0] = (seq_t)1;
            new_row[1] = (seq_t)0;
        } else if (next_row == 1) {
            new_row[0] = (seq_t)1;
            new_row[1] = (seq_t)1;
            new_row[2] = (seq_t)0;
        } else { /* next_row >= 2 */
            new_row[0] = (seq_t)1;                     /* T(i,0)=1             */
            new_row[1] = (seq_t)(next_row - 1);        /* T(i,1)=i-1           */
            /* middle entries */
            for (size_t j = 2; j <= (size_t)next_row; ++j)
            {
                seq_t a = prev_row[j-2];
                seq_t b = prev_row[j-1];
                seq_t c = (j < prev_len) ? prev_row[j] : (seq_t)0;
                new_row[j] = a + b + c;
            }
            /* last entry */
            new_row[next_row+1] = prev_row[next_row-1] + prev_row[next_row];
        }

        /* ----- append the new row to the flattened array ----- */
        ensure_data_capacity(data_len + row_len);
        for (size_t i = 0; i < row_len; ++i)
            data[data_len++] = new_row[i];

        /* ----- advance state ----- */
        free(prev_row);
        prev_row = new_row;
        prev_len = row_len;
        last_row = next_row;
    }

    return data[target];
}
