/*
 * A130136  Triangle read by rows: T(n,k) is the number of Fibonacci binary
 *          words of length n and having k 0110's (n>=0,
 *          0<=k<=floor((n-1)/3)). A Fibonacci binary word is a binary word
 *          having no 00 subword.
 * https://oeis.org/A130136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = number of Fibonacci binary words of length n containing a
 *            number k of the pattern 0110, flattened by rows (i.e., a list of
 *            all T(n,k) for n>=0, 0<=k<=floor((n-1)/3))
 * Range:     u8 n <= 30, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------------- */
/* Standard headers (no floating point, no std::).                           */
/* ------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/* Static storage for the memoised sequence and the DP tables.                */
/* ------------------------------------------------------------------------- */
static seq_t *a_arr = NULL;          /* flattened sequence */
static size_t a_cap = 0;             /* capacity of a_arr          */
static idx_t a_len = 0;              /* number of elements stored  */

static seq_t *dp_prev = NULL;        /* DP table for length cur_len */
static seq_t *dp_cur  = NULL;        /* DP table for length cur_len+1 */
static size_t dp_cap_k = 0;          /* number of k‑values currently allocated */
static idx_t  cur_len = 0;           /* length for which dp_prev is valid */

/* ------------------------------------------------------------------------- */
/* Grow a_arr to fit at least 'need' elements (need is an idx_t).            */
/* ------------------------------------------------------------------------- */
static void ensure_a_capacity(idx_t need)
{
    if ((size_t)need <= a_cap)
        return;
    size_t new_cap = a_cap ? a_cap : 1;
    while (new_cap < (size_t)need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    a_arr = tmp;
    a_cap = new_cap;
}

/* ------------------------------------------------------------------------- */
/* Grow the DP tables to hold at least 'need_k' values of k (each with 8 states). */
/* ------------------------------------------------------------------------- */
static void ensure_dp_capacity(idx_t need_k)
{
    if ((size_t)need_k <= dp_cap_k)
        return;
    size_t new_cap = dp_cap_k ? dp_cap_k : 1;
    while (new_cap < (size_t)need_k)
        new_cap <<= 1;
    size_t elems = new_cap * 8;

    seq_t *tmp_prev = (seq_t *)realloc(dp_prev, elems * sizeof(seq_t));
    seq_t *tmp_cur  = (seq_t *)realloc(dp_cur,  elems * sizeof(seq_t));
    if (!tmp_prev || !tmp_cur)
        abort();

    /* zero new entries of both tables */
    size_t old_elems = dp_cap_k * 8;
    for (size_t i = old_elems; i < elems; ++i) {
        tmp_prev[i] = 0;
        tmp_cur[i]  = 0;
    }

    dp_prev = tmp_prev;
    dp_cur  = tmp_cur;
    dp_cap_k = new_cap;
}

/* ------------------------------------------------------------------------- */
/* Extend the precomputation to include all rows up to 'target' (inclusive).  */
/* ------------------------------------------------------------------------- */
static void extend_up_to(idx_t target)
{
    /* First call – initialise DP for length 0 */
    if (dp_prev == NULL) {
        ensure_dp_capacity(1);                 /* at least one k‑value (k=0) */
        for (size_t i = 0; i < dp_cap_k * 8; ++i)
            dp_prev[i] = 0;
        dp_prev[0] = 1;                        /* empty word: one state (state 0) */
        cur_len = 0;
    }

    /* Emit row for length 0 (if not already emitted) */
    if (a_len == 0) {
        seq_t sum = 0;
        for (int s = 0; s < 8; ++s)
            sum += dp_prev[0 * 8 + s];
        ensure_a_capacity(a_len + 1);
        a_arr[a_len++] = sum;                   /* a(0) = 1 */
    }

    /* Iteratively build rows 1 .. target */
    while (cur_len < target) {
        idx_t next_len   = cur_len + 1;
        idx_t kmax_next  = next_len / 3;        /* capacity needed for next table */
        idx_t need_k     = kmax_next + 1;       /* k ranges 0 .. kmax_next */

        ensure_dp_capacity(need_k);
        /* zero the new table for the needed range */
        for (size_t i = 0; i < need_k * 8; ++i)
            dp_cur[i] = 0;

        /* Transition from length cur_len to next_len */
        idx_t kmax_prev = (cur_len == 0) ? 0 : (cur_len - 1) / 3;
        for (idx_t k = 0; k <= kmax_prev; ++k) {
            for (int s = 0; s < 8; ++s) {
                seq_t val = dp_prev[k * 8 + s];
                if (!val)
                    continue;

                /* Append '1' */
                int new_state1 = ((s << 1) & 7) | 1;
                dp_cur[k * 8 + new_state1] += val;

                /* Append '0' – only if it doesn't produce 00 */
                bool allow_zero = (cur_len == 0) ? true : ((s & 1) == 1);
                if (allow_zero) {
                    int new_state0 = (s << 1) & 7;
                    idx_t new_k = k;
                    if (s == 3 && cur_len >= 3)   /* 0110 pattern found */
                        new_k = k + 1;
                    dp_cur[new_k * 8 + new_state0] += val;
                }
            }
        }

        /* Emit the row for next_len */
        idx_t kmax_row = (next_len == 0) ? 0 : (next_len - 1) / 3;
        for (idx_t k = 0; k <= kmax_row; ++k) {
            seq_t sum = 0;
            for (int s = 0; s < 8; ++s)
                sum += dp_cur[k * 8 + s];
            ensure_a_capacity(a_len + 1);
            a_arr[a_len++] = sum;
        }

        /* Prepare for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
        cur_len = next_len;
    }
}

/* ------------------------------------------------------------------------- */
/* Public entry point – returns the n-th term of the flattened triangle.     */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (a_arr == NULL)
        extend_up_to(n);
    else if (n >= a_len)
        extend_up_to(n);
    return a_arr[n];
}
