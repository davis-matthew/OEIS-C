/*
 * A202773  Number of (n+2)X6 binary arrays avoiding patterns 000 and 011 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) – number of (n+2)×6 binary arrays avoiding patterns * 000
 *            and 011 in every row, column, and northwest‑to‑southeast *
 *            diagonal. The value is computed by dynamic programming over *
 *            admissible 6‑bit rows and their legal vertical/diagonal
 *            extensions
 * Range:     u8 none, u16 n <= 6, u32 n <= 24, u64 n <= 64, u128 n <= 143,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes, all types are provided by the build system. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Number of columns in the array. */
#define MAX_COLS 6
/* All 6‑bit masks. */
#define MAX_MASKS (1 << MAX_COLS)
/* For convenience in transition tables. */
#define MAX_VALID 64
#define MAX_STATES (MAX_VALID * MAX_VALID)

/* List of all admissible row masks (no 000 or 011 horizontally). */
static int  valid_masks[MAX_VALID];
static int  valid_count = 0;
static int  mask_index[MAX_MASKS] = {0};          /* -1 if mask is invalid */

/* Transition table: for each pair (i,j) store all k such that
 * rows i, j, k satisfy the vertical and diagonal constraints.
 * The table holds indices of the next state (j,k). */
static int   trans_to[MAX_STATES * MAX_VALID];
static int   trans_count[MAX_STATES];
static int   states = 0;

/* DP arrays holding counts for the current and next row sets. */
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;
/* Number of rows processed so far (current state has that many rows). */
static idx_t current_rows = 2;                    /* start with two rows */

/* Cached a(n) values.  a_values[0] holds a(1), etc. */
static seq_t *a_values = NULL;
static idx_t a_cap     = 0;                      /* capacity of a_values */
static idx_t done_n    = 0;                      /* largest n already cached */

/* Ensure that a_values can hold at least 'needed' elements. */
static void ensure_a_capacity(idx_t needed)
{
    if (needed <= a_cap) return;
    size_t old_cap_size = (size_t)a_cap;
    size_t new_cap = (size_t)a_cap > 0 ? (size_t)a_cap : 64;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *tmp = seq_realloc(a_values, old_cap_size, new_cap);
    a_values = tmp;
    a_cap = (idx_t)new_cap;
}

/* One‑time initialization of masks, transition tables, and DP buffers. */
static void init(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* Build the list of admissible row masks. */
    for (int mask = 0; mask < MAX_MASKS; ++mask) {
        bool ok = true;
        for (int i = 0; i <= MAX_COLS - 3; ++i) {
            int b0 = (mask >> i)     & 1;
            int b1 = (mask >> (i+1)) & 1;
            int b2 = (mask >> (i+2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                (b0 == 0 && b1 == 1 && b2 == 1))
            {
                ok = false;
                break;
            }
        }
        if (ok) {
            mask_index[mask] = valid_count;
            valid_masks[valid_count++] = mask;
        } else {
            mask_index[mask] = -1;
        }
    }

    states = valid_count * valid_count;

    /* Precompute transitions: for each pair (i,j) list all k that keep
     * rows i, j, k valid with respect to columns and NW→SE diagonals. */
    for (int i = 0; i < valid_count; ++i) {
        for (int j = 0; j < valid_count; ++j) {
            int a = valid_masks[i];
            int b = valid_masks[j];
            int s = i * valid_count + j;
            int cnt = 0;
            for (int k = 0; k < valid_count; ++k) {
                int c = valid_masks[k];
                bool ok = true;

                /* Column constraints (vertical triples). */
                for (int col = 0; col < MAX_COLS; ++col) {
                    int d0 = (a >> col) & 1;
                    int d1 = (b >> col) & 1;
                    int d2 = (c >> col) & 1;
                    if ((d0 == 0 && d1 == 0 && d2 == 0) ||
                        (d0 == 0 && d1 == 1 && d2 == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;

                /* NW→SE diagonal constraints. */
                for (int start = 0; start <= MAX_COLS - 3; ++start) {
                    int d0 = (a >> start)     & 1;
                    int d1 = (b >> (start+1)) & 1;
                    int d2 = (c >> (start+2)) & 1;
                    if ((d0 == 0 && d1 == 0 && d2 == 0) ||
                        (d0 == 0 && d1 == 1 && d2 == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;

                int next_state = j * valid_count + k;
                trans_to[s * MAX_VALID + cnt] = next_state;
                ++cnt;
            }
            trans_count[s] = cnt;
        }
    }

    /* Allocate DP arrays and initialise them: all pair choices are valid. */
    dp_cur  = seq_alloc(states);
    dp_next = seq_alloc(states);
    for (int s = 0; s < states; ++s) dp_cur[s] = 1;
}

/* --------------------------------------------------------------------- */
/* PUBLIC FUNCTION: a(n) – number of (n+2)×6 binary arrays with the
 * forbidden patterns 000 and 011 in rows, columns and NW→SE diagonals. */
seq_t a(idx_t n)
{
    init();
    if (n <= done_n) return a_values[n - 1];

    idx_t target_rows = n + 2;
    /* Ensure we have at least the first a_values element. */
    if (a_cap == 0) {
        ensure_a_capacity(1);
        a_values[0] = 0;      /* placeholder, will be overwritten */
    }

    while (current_rows < target_rows) {
        /* Reset dp_next to zero. */
        for (int i = 0; i < states; ++i) dp_next[i] = 0;

        /* Transition step. */
        for (int s = 0; s < states; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            int offset = s * MAX_VALID;
            int cnt    = trans_count[s];
            for (int t = 0; t < cnt; ++t) {
                int next_state = trans_to[offset + t];
                dp_next[next_state] += val;
            }
        }

        /* Swap buffers: dp_cur now holds counts for the new row count. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        ++current_rows;

        /* Compute total for current_rows rows and store a(n). */
        seq_t total = 0;
        for (int i = 0; i < states; ++i) total += dp_cur[i];
        idx_t n_val = current_rows - 2;   /* (rows = n+2) */
        if (n_val >= 1) {
            ensure_a_capacity(n_val);
            a_values[n_val - 1] = total;
        }
    }

    done_n = n;
    return a_values[n - 1];
}
