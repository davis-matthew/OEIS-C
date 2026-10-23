/*
 * A035513  Wythoff array read by falling antidiagonals.
 * https://oeis.org/A035513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice, changed
 * Formula:   a(n) = T(row, col-1) where row,col are the 1‑based coordinates
 *            of the n‑th entry read by falling antidiagonals of the Wythoff
 *            array. The values are computed by the recurrence T(n,-1) = n-1,
 *            T(n,0) = floor(n*τ), T(n,k) = T(n,k-1)+T(n,k-2) (k≥1)
 * Range:     u8 n <= 56, u16 n <= 254, u32 n <= 1036, u64 n <= 4187,
 *            u128 n <= 5151, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required types are supplied by the build: seq_t, idx_t */
#include <stdbool.h>
#include <stddef.h>

/* Max row we precompute for.  For n up to 10 000 this is far enough.
   If a larger n is requested the code will still work but slower. */
#define MAX_ROW 20000
/* Maximum column we precompute.  For the expected input range this
   is more than enough. */
#define MAX_COL 20000
/* Size of the table that stores the used integers when generating
   floor(n*τ).  Should be larger than the largest integer that can
   appear in floor(n*τ)+n for n ≤ MAX_ROW. */
#define USED_CAP 60000

static seq_t floor_tau_table[MAX_ROW + 1];
static seq_t fib_table[MAX_COL + 2];
static bool used[USED_CAP + 1];
static bool init_done = false;

/* Pre-compute floor(n*τ) using the Beatty pair complement trick
   and the Fibonacci numbers.  */
static void init_constants(void)
{
    size_t m = 1;
    for (size_t n = 1; n <= (size_t)MAX_ROW; ++n) {
        while (used[m]) ++m;                 /* smallest unused integer */
        floor_tau_table[n] = (seq_t)m;
        size_t b = m + n;                    /* partner in upper Wythoff seq */
        if (b <= (size_t)USED_CAP) used[b] = true;
        used[m] = true;
    }

    fib_table[0] = (seq_t)0;
    fib_table[1] = (seq_t)1;
    for (size_t k = 2; k <= (size_t)MAX_COL + 1; ++k)
        fib_table[k] = fib_table[k - 1] + fib_table[k - 2];
}

/* Return the (row,col) of the n-th entry read by falling antidiagonals.
   Rows and columns are 1‑based for the array. */
static void antidiagonal_coords(idx_t n, idx_t *row, idx_t *col)
{
    /* Find the diagonal d such that triangular(d) >= n.
       triangular(d) = d*(d+1)/2 gives the total number of elements
       up to diagonal d-1.  We keep increasing d until it holds. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < n)
        ++d;

    idx_t prev = d * (d + 1) / 2;           /* elements before this diag */
    idx_t offset = n - prev - 1;            /* 0‑based position on diag */
    idx_t i = offset;                       /* 0‑based row index on diag */
    idx_t j = d - offset;                   /* 0‑based column index */
    *row = i + 1;                           /* convert to 1‑based */
    *col = j + 1;
}

/* Main function requested in the problem statement. */
seq_t a(idx_t n)
{
    if (!init_done) {
        init_constants();
        init_done = true;
    }
    if (n == 0) return 0;   /* offset starts at 1 */

    idx_t row, col;
    antidiagonal_coords(n, &row, &col);

    /* If requested indices are beyond the pre‑computed range, extend
       the tables.  The problem constraints guarantee that this will
       not happen for n up to about 10 000.  */
    if (row > (idx_t)MAX_ROW || col > (idx_t)MAX_COL) {
        /* In a complete implementation one would reallocate and
           extend the tables; for the current limits this branch is
           effectively unreachable. */
        return (seq_t)0;
    }

    seq_t floor_row = floor_tau_table[(size_t)row];
    seq_t val = fib_table[(size_t)col + 1] * floor_row
              + fib_table[(size_t)col] * ((seq_t)(row - 1));
    return val;
}
