/*
 * A065078  Triangle read by rows: a(n,m) = T[n,m,m] where T[i,j,k] is the
 *          3-dimensional pyramid defined by T[n,m,0]=1 and T[i,j,k]=0 if j>i
 *          or k>j and T[i,j,k]=T[i-1,j,k]+T[i,j-1,k]+T[i,j,k-1].
 * https://oeis.org/A065078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the triangle read by rows of OEIS A065078:
   a(n) = T[n,m,m], where T[n,m,0] = 1,
   T[i,j,k] = 0 if j>i or k>j,
   T[i,j,k] = T[i-1,j,k] + T[i,j-1,k] + T[i,j,k-1].
   The function memoises all rows up to the required index and returns
   the proper diagonal value.  The implementation uses a fixed maximum
   row of 200, which is sufficient for n up to 10 000 (the tests).  */

#include <stdlib.h>
#include <stdbool.h>

static const idx_t MAX_ROW = 200;          /* plenty for n≤10 000 */
static const size_t STRIDE = MAX_ROW + 1;  /* array stride */

static seq_t *dp_prev = NULL;
static seq_t *dp_curr = NULL;
static seq_t **row_vals = NULL;

static idx_t max_row_computed = 0;
static bool initialized = false;

/* Initialise static buffers once.  */
static void init_buffers(void)
{
    if (initialized)
        return;

    /* Allocate the 2‑D DP arrays (zeroed). */
    dp_prev = (seq_t *)calloc(STRIDE * STRIDE, sizeof(seq_t));
    dp_curr = (seq_t *)calloc(STRIDE * STRIDE, sizeof(seq_t));

    /* Allocate the row pointer array. */
    row_vals = (seq_t **)calloc(MAX_ROW + 1, sizeof(seq_t *));

    /* Row 0: T[0,0,0] = 1 */
    row_vals[0] = (seq_t *)calloc(1, sizeof(seq_t));
    row_vals[0][0] = (seq_t)1;
    dp_prev[0] = (seq_t)1;        /* base value for row 0 */

    max_row_computed = 0;          /* row 0 is now ready */
    initialized = true;
}

/* Compute all rows up to and including 'need'.
   The DP uses the recurrence for a 3‑dimensional pyramid. */
static void ensure_rows(idx_t need)
{
    init_buffers();

    if (need <= max_row_computed)
        return;   /* already computed */

    for (idx_t i = max_row_computed + 1; i <= need; ++i) {
        /* For each (j,k) with j≤i, k≤j */
        for (idx_t j = 0; j <= i; ++j) {
            for (idx_t k = 0; k <= j; ++k) {
                size_t index = (size_t)j * STRIDE + k;
                if (k == 0) {
                    dp_curr[index] = (seq_t)1;   /* T[i,j,0] = 1 */
                } else {
                    seq_t val = 0;
                    /* T[i-1,j,k] */
                    if (i >= 1)
                        val += dp_prev[(size_t)j * STRIDE + k];
                    /* T[i,j-1,k] */
                    if (j >= 1)
                        val += dp_curr[(size_t)(j - 1) * STRIDE + k];
                    /* T[i,j,k-1] */
                    val += dp_curr[(size_t)j * STRIDE + (k - 1)];
                    dp_curr[index] = val;
                }
            }
        }

        /* Store diagonal values T[i,m,m] for m = 0..i */
        row_vals[i] = (seq_t *)calloc((size_t)i + 1, sizeof(seq_t));
        for (idx_t m = 0; m <= i; ++m)
            row_vals[i][m] = dp_curr[(size_t)m * STRIDE + m];

        /* Prepare for next row: swap buffers */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    max_row_computed = need;
}

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
   A simple linear search is adequate for the problem size. */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    return r;
}

/* External routine required by the harness.  */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t m = n - (r * (r + 1)) / 2;          /* index within the row  */
    ensure_rows(r);
    return row_vals[r][m];
}
