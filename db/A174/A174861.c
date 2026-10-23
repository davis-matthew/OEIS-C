/*
 * A174861  Triangle read by rows: T(n,k) = n! * (Stirling1(n,k) +
 *          Stirling1(n,n-k) - 1) + 1 with T(0,0)=1.
 * https://oeis.org/A174861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less
 * Formula:   T(n,k) = n! * (S(n,k) + S(n,n-k) - 1) + 1 -- A174861
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system. */

static idx_t  max_row = 0;          /* largest n for which rows have been computed */
static size_t seq_len = 1;          /* number of a(n) values stored (index 0..seq_len-1) */

static seq_t *a_seq = NULL;         /* flattened triangle values */
static seq_t *fact  = NULL;         /* factorials: fact[i] = i! */
static seq_t *s_prev = NULL;        /* previous row of Stirling numbers */
static size_t s_prev_len = 0;       /* length of s_prev (previous n + 1) */

/* Ensure the factorial array holds up to `n`. */
static void ensure_factorial(idx_t n)
{
    if (fact == NULL) {
        fact = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!fact) abort();
        fact[0] = (seq_t)1;
    } else {
        fact = (seq_t*)realloc(fact, (n + 1) * sizeof(seq_t));
        if (!fact) abort();
    }
}

/* Ensure the flattened array can hold at least `needed` elements. */
static void ensure_a_capacity(size_t needed)
{
    if (a_seq == NULL) {
        a_seq = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!a_seq) abort();
    } else if (needed > seq_len) {
        a_seq = (seq_t*)realloc(a_seq, needed * sizeof(seq_t));
        if (!a_seq) abort();
    }
    seq_len = needed;
}

/* Compute rows up to and including `target_row`. After the call `max_row` equals `target_row`. */
static void compute_up_to(idx_t target_row)
{
    /* Initialise row 0 once. */
    if (max_row == 0 && s_prev == NULL) {
        s_prev = (seq_t*)malloc(sizeof(seq_t));
        if (!s_prev) abort();
        s_prev[0] = (seq_t)1;
        s_prev_len = 1;
        ensure_factorial(0);
        fact[0] = (seq_t)1;
    }

    ensure_factorial(target_row);

    for (idx_t r = max_row + 1; r <= target_row; ++r) {
        size_t rlen = r + 1;
        seq_t *s_cur = (seq_t*)calloc(rlen, sizeof(seq_t));
        if (!s_cur) abort();

        /* Stirling recurrence: s(n,0)=0, s(n,n)=1 */
        s_cur[0] = (seq_t)0;
        s_cur[r] = (seq_t)1;
        for (idx_t k = 1; k < r; ++k) {
            /* s(n,k) = s(n-1,k-1) - (n-1)*s(n-1,k) */
            s_cur[k] = s_prev[k - 1] - (seq_t)(r - 1) * s_prev[k];
        }

        /* Factorial for this row */
        fact[r] = fact[r - 1] * (seq_t)r;

        /* Start index of this row in the flattened array */
        idx_t row_start = (r * (r + 1)) / 2;

        /* Ensure capacity for this row */
        ensure_a_capacity(row_start + r + 1);

        /* Compute triangle entries and store them */
        for (idx_t k = 0; k <= r; ++k) {
            seq_t temp = s_cur[k] + s_cur[r - k] - (seq_t)1;
            seq_t val  = fact[r] * temp + (seq_t)1;
            a_seq[row_start + k] = val;
        }

        /* Release previous Stirling row and promote current row */
        free(s_prev);
        s_prev     = s_cur;
        s_prev_len = rlen;
        max_row    = r;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* T(0,0) = 1 by definition */
        return (seq_t)1;

    /* Ensure we have computed up to the required index. */
    if (n >= seq_len) {
        /* Determine which row contains index n. */
        idx_t r = 0;
        while (((r * (r + 1)) / 2 + r) < n)  /* while start+length-1 < n */
            ++r;
        /* Compute rows up to r. */
        compute_up_to(r);
    }

    return a_seq[n];
}
