/*
 * A129718  Triangle read by rows: T(n,k) is the number of Fibonacci binary
 *          words of length n and having k runs of 1's (n >= 0, 0 <= k <=
 *          floor((n+1)/2)). A Fibonacci binary word is a binary word having
 *          no 00 subword. A run of 1's is a maximal subword of the form
 *          11..1.
 * https://oeis.org/A129718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened triangle of Fibonacci binary word counts
 *            T(n,k): number of binary words of length n with no 00 and
 *            exactly k runs of 1's. The triangle is read by rows: index 0 =
 *            T(0,0), indices 1,2 = T(1,0),T(1,1), indices 3,4 =
 *            T(2,0),T(2,1), etc
 * Range:     u8 n <= 66, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Forward declarations of helper functions */
static void ensure_rows(idx_t need_row);
static int find_row(idx_t idx, idx_t *row, idx_t *col);

/* Storage for the triangle */
static seq_t **T_rows = NULL;       /* pointers to each row’s array */
static size_t *T_len  = NULL;       /* length of each row */
static size_t T_cap   = 0;          /* allocated capacity for T_rows/T_len */
static idx_t  last_row = (idx_t)-1; /* last row that has been fully computed */

/* Storage for A and B arrays of the last computed row (needed for recurrence) */
static seq_t *prev_a = NULL;
static seq_t *prev_b = NULL;
static size_t prev_len = 0;

/* ------------------------------------------------------------ */
/* Helper: find the triangle row and column for a global index   */
/* ------------------------------------------------------------ */
static int find_row(idx_t idx, idx_t *row, idx_t *col)
{
    idx_t r = 0;
    idx_t cum = 0;
    while (1) {
        size_t len = 1 + ((r + 1) / 2);
        idx_t next_cum = cum + (idx_t)len;
        if (idx < next_cum) {
            *row = r;
            *col = idx - cum;
            return 0;
        }
        cum = next_cum;
        ++r;
    }
    return -1; /* unreachable */
}

/* ------------------------------------------------------------ */
/* Helper: ensure that rows up to need_row have been computed   */
/* ------------------------------------------------------------ */
static void ensure_rows(idx_t need_row)
{
    /* First-time initialization: create row 0 */
    if (T_rows == NULL) {
        T_cap = 1;
        T_rows = (seq_t**)malloc(T_cap * sizeof(seq_t*));
        T_len  = (size_t*)malloc(T_cap * sizeof(size_t));

        /* Row 0: [1] */
        T_rows[0] = (seq_t*)malloc(sizeof(seq_t));
        T_rows[0][0] = 1;
        T_len[0] = 1;
        last_row = 0;

        prev_a  = NULL;
        prev_b  = NULL;
        prev_len = 0;
    }

    /* Compute missing rows up to need_row */
    while (last_row < need_row) {
        idx_t n = last_row + 1;          /* next row index */
        size_t len = 1 + ((n + 1) / 2);  /* length of the new row */

        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));

        if (n == 1) {
            /* Row 1: [1, 1] */
            row[0] = 1;
            row[1] = 1;

            /* Set up A and B arrays for row 1 */
            if (prev_a) free(prev_a);
            if (prev_b) free(prev_b);
            prev_a = (seq_t*)malloc(len * sizeof(seq_t));
            prev_b = (seq_t*)malloc(len * sizeof(seq_t));
            for (size_t i = 0; i < len; ++i) {
                prev_a[i] = 0;
                prev_b[i] = 0;
            }
            prev_a[0] = 1;
            prev_b[1] = 1;
            prev_len = len;
        } else {
            /* Row n (n >= 2): compute using recurrence */
            seq_t *curr_a = (seq_t*)calloc(len, sizeof(seq_t));
            seq_t *curr_b = (seq_t*)calloc(len, sizeof(seq_t));

            for (size_t k = 0; k < len; ++k) {
                /* A[n][k] = B[n-1][k] */
                curr_a[k] = (k < prev_len) ? prev_b[k] : 0;
                /* B[n][k] = B[n-1][k] + (k>=1 ? A[n-1][k-1] : 0) */
                seq_t term = (k < prev_len) ? prev_b[k] : 0;
                if (k >= 1 && (k - 1) < prev_len) {
                    term += prev_a[k - 1];
                }
                curr_b[k] = term;
                row[k] = curr_a[k] + curr_b[k];
            }
            free(prev_a);
            free(prev_b);
            prev_a = curr_a;
            prev_b = curr_b;
            prev_len = len;
        }

        /* Grow storage if necessary */
        if ((size_t)n >= T_cap) {
            size_t new_cap = n + 1;
            T_rows = (seq_t**)realloc(T_rows, new_cap * sizeof(seq_t*));
            T_len  = (size_t*)realloc(T_len,  new_cap * sizeof(size_t));
            T_cap  = new_cap;
        }

        T_rows[n] = row;
        T_len[n]  = len;
        last_row  = n;
    }
}

/* ------------------------------------------------------------ */
/* Main function requested by the problem statement              */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t r, c;
    find_row(n, &r, &c);          /* locate row and column */
    ensure_rows(r);               /* make sure rows are ready */
    return T_rows[r][c];          /* return the requested value */
}
