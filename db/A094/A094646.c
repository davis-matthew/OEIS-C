/*
 * A094646  Triangle of generalized Stirling numbers of the first kind read by
 *          rows: T(n, k) = (-1)^(n+k)*Sum_{m=0..n} Stirling1(n, m) *
 *          binomial(m, k) * 2^(m-k), where Stirling1 is A048994.
 * https://oeis.org/A094646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 36, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of generalized Stirling numbers of the first kind
   read by rows. The OEIS sequence a(n) is the flattened triangle:
   a(0)=1; a(1)=-2, a(2)=1; a(3)=2, a(4)=-3, a(5)=1; … */
/* Recurrence (n≥1, 0≤k≤n)
   T(n,k)=T(n‑1,k‑1)+(n‑3)*T(n‑1,k)   (T(n,–1)=0, T(0,0)=1) */

/* All big‑integer helpers are available as extern types. */
#include <stdlib.h>
#include <stdint.h>

/* Cached sequence values (row‑major). */
static seq_t *seq      = NULL; /* flattened triangle values    */
static size_t seq_cap  = 0;    /* current capacity of seq       */
static size_t seq_len  = 0;    /* number of stored elements     */

/* The last computed row. */
static seq_t *prev       = NULL; /* previous row array           */
static idx_t  prev_len   = 0;    /* length of previous row       */
static idx_t  current_row = 0;   /* index of last completed row */

/* Ensure that the allocation for seq is large enough. */
static void
ensure_seq_cap (size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq = (seq_t*)realloc (seq, new_cap * sizeof (seq_t));
        seq_cap = new_cap;
    }
}

/* Append one more row of the triangle.  The new row number is new_row. */
static void
extend_to_row (idx_t new_row)
{
    idx_t   n        = new_row;
    size_t  row_size = (size_t)n + 1;
    seq_t  *new_row_arr = (seq_t*)malloc (row_size * sizeof (seq_t));
    seq_t   nn = (seq_t)n;   /* signed version of the row number */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t prev_k_minus1 = (k > 0) ? prev[k-1] : 0;
        seq_t prev_k        = (k < prev_len) ? prev[k] : 0;
        seq_t val = prev_k_minus1 + (nn - 3) * prev_k;

        new_row_arr[k] = val;

        ensure_seq_cap (seq_len + 1);
        seq[seq_len++] = val;
    }

    free (prev);
    prev = new_row_arr;
    prev_len = (idx_t)row_size;
    current_row = n;
}

/* Expand the memoised sequence so that elements up to index n exist. */
static void
ensure_up_to (idx_t n)
{
    if (seq == NULL) { /* first call – initialise row 0 */
        seq_cap = 8;
        seq = (seq_t*)malloc (seq_cap * sizeof (seq_t));
        seq_len = 0;

        /* Row 0: T(0,0)=1 */
        prev_len = 1;
        prev = (seq_t*)malloc (sizeof (seq_t));
        prev[0] = 1;
        ensure_seq_cap (1);
        seq[seq_len++] = 1;
        current_row = 0;
    }

    while (seq_len <= n)                 /* one more element needed   */
        extend_to_row (current_row + 1);
}

/* Public function : returns the nth term of the flattened triangle. */
seq_t
a (idx_t n)
{
    ensure_up_to (n);
    return seq[(size_t)n];
}
