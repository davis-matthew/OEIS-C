/*
 * A157278  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) + m*f(n,k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1, f(n, k) = 2*k if k <= floor(n/2) otherwise 2*(n-k), and
 *          m = 3, read by rows.
 * https://oeis.org/A157278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 57, u64 n <= 141,
 *            u128 n <= 387, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Defines the triangle T(n,k,3) by
   T(n,0)=T(n,n)=1,
   T(n,k) = (3*(n-k)+1)*T(n-1,k-1) + (3*k+1)*T(n-1,k) + 3*f(n,k)*T(n-2,k-1),
   where f(n,k)=2*k for k≤⌊n/2⌋, else f(n,k)=2*(n-k).
   The sequence a(n) is obtained by listing the entries of T row by row
   (row 0: T(0,0), row 1: T(1,0),T(1,1), …). */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_vals   = NULL;   /* array of a(0),a(1),…   */
static size_t seq_len    = 0;      /* number of values in seq_vals   */
static size_t seq_cap    = 0;      /* capacity of seq_vals           */

static seq_t *prev_row   = NULL;   /* triangle row n-1           */
static size_t prev_len   = 0;
static seq_t *prev2_row  = NULL;   /* triangle row n-2           */
static size_t prev2_len  = 0;
static size_t computed_rows = 0;   /* number of rows already computed */

/* Append a value to seq_vals, growing the array if necessary */
static void push_to_seq(seq_t val)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap < seq_len + 1) new_cap = seq_len + 1;
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t *>(
            realloc(seq_vals, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
#endif
        if (!tmp) abort(); /* out of memory */
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* Generate the next triangle row (index = computed_rows) and append it to seq_vals. */
static void compute_next_row(void)
{
    size_t row_index = computed_rows;          /* zero‑based row number */
    size_t cur_len   = row_index + 1;
#ifdef __cplusplus
    seq_t *cur = new seq_t[cur_len];
#else
    seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
#endif
    if (!cur) abort();

    cur[0] = (seq_t)1;
    cur[row_index] = (seq_t)1;                 /* boundary values */

    if (row_index >= 2) {                      /* recurrence for interior k */
        for (size_t k = 1; k < row_index; ++k) {
            seq_t term1 = ((seq_t)3 * (seq_t)(row_index - k) + (seq_t)1) * prev_row[k - 1];
            seq_t term2 = ((seq_t)3 * (seq_t)k + (seq_t)1) * prev_row[k];
            seq_t f;
            if (k <= row_index / 2)
                f = (seq_t)2 * (seq_t)k;
            else
                f = (seq_t)2 * (seq_t)(row_index - k);
            seq_t term3 = ((seq_t)3 * f) * prev2_row[k - 1];
            cur[k] = term1 + term2 + term3;
        }
    }

    /* Store the row entries into the global sequence array */
    for (size_t k = 0; k < cur_len; ++k)
        push_to_seq(cur[k]);

    /* Slide the row buffers: discard old prev2, shift previous rows */
#ifdef __cplusplus
    delete[] prev2_row;
#else
    free(prev2_row);
#endif
    prev2_row  = prev_row;
    prev2_len  = prev_len;
    prev_row   = cur;
    prev_len   = cur_len;

    ++computed_rows;
}

/* Ensure that seq_vals contains at least indices 0 … n. */
static void compute_until(idx_t n)
{
    while (seq_len <= (size_t)n)
        compute_next_row();
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    compute_until(n);
    return seq_vals[(size_t)n];
}
