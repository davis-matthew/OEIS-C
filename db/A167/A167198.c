/*
 * A167198  Fractal sequence of the interspersion A083047.
 * https://oeis.org/A167198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A167198 – Fractal sequence constructed by rows.
   Row 1: 1
   Row 2: 1
   Row 3: 2,1
   Row 4: 2,3,1
   For n ≥ 4, to form row n+1:
      k = smallest unused integer
      For each element v of row n, if v occurs in row n‑1,
      insert k immediately before v and increment k.
   A167198 is the concatenation of all rows. */

#include <stdlib.h>
#include <stdbool.h>

/* Static state for memoised generation */
static seq_t *seq      = NULL;     /* concatenated sequence           */
static idx_t  seq_len  = 0;        /* number of terms stored          */
static size_t seq_cap  = 0;        /* capacity for seq                */

static seq_t *row_prevprev = NULL; /* row r‑2                          */
static size_t len_prevprev = 0;
static seq_t *row_prev     = NULL; /* row r‑1                          */
static size_t len_prev     = 0;

static idx_t next_unused = 0;      /* smallest positive integer not yet used */
static bool initialized = false;

/* Grow seq to at least new_cap elements. */
static void ensure_seq_capacity(size_t new_cap)
{
    if (new_cap > seq_cap) {
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_arr == NULL) abort();
        for (size_t i = 0; i < seq_len; ++i) new_arr[i] = seq[i];
        free(seq);
        seq = new_arr;
        seq_cap = new_cap;
    }
}

/* Test whether v appears in row_prevprev. */
static bool in_prevprev(seq_t v)
{
    for (size_t j = 0; j < len_prevprev; ++j)
        if (row_prevprev[j] == v) return true;
    return false;
}

/* Initialise static data with the first four rows. */
static void init_sequence(void)
{
    seq_cap = 64;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;

    /* Row 1 */
    ensure_seq_capacity(seq_len + 1);
    seq[seq_len++] = (seq_t)1;

    /* Row 2 */
    ensure_seq_capacity(seq_len + 1);
    seq[seq_len++] = (seq_t)1;

    /* Row 3 */
    ensure_seq_capacity(seq_len + 2);
    seq[seq_len++] = (seq_t)2;
    seq[seq_len++] = (seq_t)1;

    /* Row 4 */
    ensure_seq_capacity(seq_len + 3);
    seq[seq_len++] = (seq_t)2;
    seq[seq_len++] = (seq_t)3;
    seq[seq_len++] = (seq_t)1;

    /* Store row 3 and 4 for future rows. */
    len_prevprev = 2;
    row_prevprev = (seq_t *)malloc(len_prevprev * sizeof(seq_t));
    row_prevprev[0] = (seq_t)2;
    row_prevprev[1] = (seq_t)1;

    len_prev = 3;
    row_prev = (seq_t *)malloc(len_prev * sizeof(seq_t));
    row_prev[0] = (seq_t)2;
    row_prev[1] = (seq_t)3;
    row_prev[2] = (seq_t)1;

    next_unused = 4;
    initialized = true;
}

/* Generate rows until seq_len ≥ n. */
static void generate_until(idx_t n)
{
    while (seq_len < n) {
        /* Count how many elements of row_prev occur in row_prevprev. */
        size_t matches = 0;
        for (size_t i = 0; i < len_prev; ++i)
            if (in_prevprev(row_prev[i])) ++matches;

        size_t new_len = len_prev + matches;
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_row == NULL) abort();

        size_t j = 0;
        seq_t k = (seq_t)next_unused;   /* start of new numbers */

        for (size_t i = 0; i < len_prev; ++i) {
            seq_t val = row_prev[i];
            if (in_prevprev(val)) {
                new_row[j++] = k++;
            }
            new_row[j++] = val;
        }

        /* Append the new row to the global sequence. */
        ensure_seq_capacity(seq_len + new_len);
        for (size_t i = 0; i < new_len; ++i)
            seq[seq_len++] = new_row[i];

        /* Advance row buffers. */
        free(row_prevprev);
        row_prevprev = row_prev;
        len_prevprev = len_prev;

        row_prev = new_row;
        len_prev = new_len;

        next_unused = (idx_t)k;
    }
}

/* Public access function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* ignore requests below offset */
    if (!initialized) init_sequence();
    if (seq_len < n) generate_until(n);
    return seq[n - 1];
}
