/*
 * A164191  Number of binary strings of length n with equal numbers of 00000
 *          and 11111 substrings.
 * https://oeis.org/A164191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n containing the same
 *            number of 00000 and 11111 substrings
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming over an automaton containing the last
   four bits and the difference (#00000 − #11111). The DP increments
   the difference only when a new 00000 or 11111 substring appears. */
/* The algorithm keeps the result table in a static array and extends it
   lazily as larger n are requested.  The two DP buffers are resized
   whenever a larger maximum n is encountered.  The difference variable
   is stored with an offset SHIFT = maximum_n, so that the index for
   difference d is SHIFT + d.  This ensures all accesses stay within
   bounds. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const int NUM_MASKS = 16;        /* 4-bit window */
static const int MAX_MASK = NUM_MASKS - 1;

/* DP state buffers and table */
static seq_t *curr_buf = NULL;
static seq_t *next_buf = NULL;
static size_t diff_size = 0;           /* current allocated size of diff dimension */
static size_t shift_pos = 0;           /* index of diff = 0 */
static seq_t *table = NULL;
static size_t table_cap = 0;
static idx_t highest_computed = 0;

/* Ensure DP buffers are large enough for a maximum n */
static void init_buffers(idx_t max_n)
{
    if (curr_buf != NULL)
        return;                     /* already initialized */

    /* shift position is the maximum n requested */
    shift_pos = (size_t)max_n;
    diff_size = 2 * (size_t)max_n + 1;   /* indices 0 .. 2*n */

    size_t total = (size_t)NUM_MASKS * diff_size;
    curr_buf = (seq_t *)calloc(total, sizeof(seq_t));
    next_buf = (seq_t *)calloc(total, sizeof(seq_t));
    if (!curr_buf || !next_buf)
        exit(EXIT_FAILURE);

    table = (seq_t *)calloc(1, sizeof(seq_t));
    if (!table)
        exit(EXIT_FAILURE);
    table_cap = 1;

    /* a(0) = 1 */
    table[0] = 1;
    table_cap = 1;
    highest_computed = 0;

    /* initial DP state for length 0: only one string, mask 0, diff 0 */
    curr_buf[shift_pos] = 1;
}

/* Resize DP buffers when a larger maximum n is requested */
static void resize_buffers(idx_t new_max_n)
{
    if (curr_buf != NULL && new_max_n <= (idx_t)shift_pos)
        return;                     /* current buffers already large enough */

    size_t new_diff_size = 2 * (size_t)new_max_n + 1;
    size_t new_shift = (size_t)new_max_n;
    size_t new_total  = (size_t)NUM_MASKS * new_diff_size;

    seq_t *new_curr = (seq_t *)calloc(new_total, sizeof(seq_t));
    seq_t *new_next = (seq_t *)calloc(new_total, sizeof(seq_t));
    if (!new_curr || !new_next)
        exit(EXIT_FAILURE);

    /* copy existing state for the current highest length */
    idx_t len = highest_computed;
    int dmax = (len >= 5) ? (int)(len - 4) : 0;
    for (int m = 0; m < NUM_MASKS; ++m) {
        size_t old_base = (size_t)m * diff_size;
        size_t new_base = (size_t)m * new_diff_size;
        for (int d = -dmax; d <= dmax; ++d) {
            size_t old_idx = old_base + shift_pos + d;
            size_t new_idx = new_base + new_shift + d;
            new_curr[new_idx] = curr_buf[old_idx];
        }
    }

    free(curr_buf);
    free(next_buf);
    curr_buf = new_curr;
    next_buf = new_next;
    diff_size = new_diff_size;
    shift_pos  = new_shift;
}

/* Ensure table is large enough */
static void ensure_table_cap(idx_t n)
{
    if ((size_t)n + 1 > table_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_tbl = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
        if (!new_tbl)
            exit(EXIT_FAILURE);
        memset(&new_tbl[table_cap], 0, (new_cap - table_cap) * sizeof(seq_t));
        table = new_tbl;
        table_cap = new_cap;
    }
}

/* Compute all values up to n */
static void compute_up_to(idx_t n)
{
    init_buffers(n);

    if (n <= highest_computed)
        return;                     /* already computed */

    /* Extend buffers if new maximum n is larger */
    if ((idx_t)shift_pos < n)
        resize_buffers(n);

    for (idx_t len = highest_computed; len < n; ++len) {
        int curr_dmax = (len >= 5) ? (int)(len - 4) : 0;
        int next_dmax = ((len + 1) >= 5) ? (int)(len + 1 - 4) : 0;

        /* clear next buffer region that will be used */
        size_t next_start = shift_pos - next_dmax;
        size_t next_len   = (size_t)(2 * next_dmax + 1);
        for (int m = 0; m < NUM_MASKS; ++m) {
            size_t base = (size_t)m * diff_size;
            memset(&next_buf[base + next_start], 0, next_len * sizeof(seq_t));
        }

        /* transition */
        for (int m = 0; m < NUM_MASKS; ++m) {
            size_t base = (size_t)m * diff_size;
            for (int d = -curr_dmax; d <= curr_dmax; ++d) {
                size_t idx = base + shift_pos + d;
                seq_t val = curr_buf[idx];
                if (val == 0)
                    continue;

                /* append 0 */
                int nmask0 = (m << 1) & 0xF;
                int delta0 = (len >= 4 && m == 0) ? 1 : 0;
                size_t idx0 = (size_t)nmask0 * diff_size + shift_pos + (d + delta0);
                next_buf[idx0] += val;

                /* append 1 */
                int nmask1 = ((m << 1) | 1) & 0xF;
                int delta1 = (len >= 4 && m == MAX_MASK) ? -1 : 0;
                size_t idx1 = (size_t)nmask1 * diff_size + shift_pos + (d + delta1);
                next_buf[idx1] += val;
            }
        }

        /* sum over diff = 0 for all masks */
        seq_t sum = 0;
        for (int m = 0; m < NUM_MASKS; ++m)
            sum += next_buf[(size_t)m * diff_size + shift_pos];

        ensure_table_cap((idx_t)len + 1);
        table[(size_t)len + 1] = sum;

        /* prepare for next iteration */
        seq_t *tmp = curr_buf;
        curr_buf = next_buf;
        next_buf = tmp;
    }

    highest_computed = n;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return table[(size_t)n];
}
