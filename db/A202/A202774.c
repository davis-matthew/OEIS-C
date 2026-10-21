/*
 * A202774  Number of (n+2)X7 binary arrays avoiding patterns 000 and 011 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 23, u64 n <= 62, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of (n+2)×7 binary arrays avoiding patterns 000 and 011
   in rows, columns and northwest‑to‑southeast diagonals.
   The array is counted for n ≥ 1 (offset 1).          */

#include <stdlib.h>
#include <stddef.h>

/* External linkage required by the build. */
seq_t a(idx_t n);

/* ----------------------------------------------------------- */
/* Internal tables.                                           */
/* ----------------------------------------------------------- */

static unsigned char *valid_masks;          /* list of masks that are row‑valid */
static unsigned char mask_to_index[128];    /* map mask → index, 255 if none   */
static idx_t V;                            /* number of valid masks          */

static idx_t Nstates;                       /* V × V of state pairs           */
static unsigned char *trans_outcnt;          /* outgoing transitions per state */
static unsigned char *trans_dest;            /* flattened destination lists     */

static seq_t *dp_cur, *dp_next;             /* DP state arrays                 */
static idx_t processed_rows;                /* rows processed so far           */
static idx_t max_prepared_n;                /* largest n whose value cached    */
static seq_t *results;                      /* results[1..max_prepared_n]      */
static size_t results_cap;                  /* capacity of results array       */

static int initialized = 0;                 /* tables ready flag               */

/* ----------------------------------------------------------- */
/* Build lookup tables.                                       */
/* ----------------------------------------------------------- */
static void init_tables(void)
{
    /* Build list of row‑valid masks (no horizontal 000 or 011). */
    unsigned char masks[128];
    size_t cnt = 0;
    for (int m = 0; m < 128; ++m) {
        int ok = 1;
        for (int i = 0; i <= 4; ++i) {
            unsigned char t = (m >> i) & 7;          /* 3 consecutive bits   */
            if (t == 0 || t == 6) {                /* 000 or 011, horizontal */
                ok = 0;
                break;
            }
        }
        if (ok) {
            masks[cnt++] = (unsigned char)m;
        }
    }
    V = (idx_t)cnt;

    /* Map mask to index. */
    for (int i = 0; i < 128; ++i) mask_to_index[i] = 255;
    for (size_t i = 0; i < V; ++i) mask_to_index[masks[i]] = (unsigned char)i;
    valid_masks = (unsigned char*)malloc(V * sizeof(unsigned char));
    for (size_t i = 0; i < V; ++i) valid_masks[i] = masks[i];

    /* Build transitions. */
    Nstates = V * V;
    trans_outcnt = (unsigned char*)malloc(Nstates * sizeof(unsigned char));
    for (size_t i = 0; i < Nstates; ++i) trans_outcnt[i] = 0;

    trans_dest = (unsigned char*)malloc(Nstates * V * sizeof(unsigned char));
    for (size_t s = 0; s < Nstates; ++s) {
        unsigned char *dest_arr = trans_dest + s * V;
        unsigned char a_idx = (unsigned char)(s / V);
        unsigned char b_idx = (unsigned char)(s % V);
        unsigned int a = valid_masks[a_idx];
        unsigned int b = valid_masks[b_idx];

        for (unsigned char c_idx = 0; c_idx < V; ++c_idx) {
            unsigned int c = valid_masks[c_idx];
            int ok = 1;
            /* vertical triples a,b,c */
            for (int j = 0; j < 7 && ok; ++j) {
                int triple = ((a >> j) & 1) * 4 + ((b >> j) & 1) * 2 + ((c >> j) & 1);
                if (triple == 0 || triple == 3) ok = 0;
            }
            /* NW-SE diagonal triples */
            for (int j = 0; j <= 4 && ok; ++j) {
                int triple = ((a >> j) & 1) * 4 + ((b >> (j + 1)) & 1) * 2
                             + ((c >> (j + 2)) & 1);
                if (triple == 0 || triple == 3) ok = 0;
            }
            if (ok) {
                dest_arr[trans_outcnt[s]++] = c_idx;
            }
        }
    }

    /* Allocate DP arrays. */
    dp_cur  = (seq_t*)malloc(Nstates * sizeof(seq_t));
    dp_next = (seq_t*)malloc(Nstates * sizeof(seq_t));
    for (size_t i = 0; i < Nstates; ++i) {
        dp_cur[i]  = 0;
        dp_next[i] = 0;
    }

    /* Initialize for first two rows: all pairs of valid masks count as 1. */
    for (size_t s = 0; s < Nstates; ++s) {
        dp_cur[s] = 1;
    }
    processed_rows = 2;       /* two rows processed */
    max_prepared_n = 0;
    results = NULL;
    results_cap = 0;
    initialized = 1;
}

/* ----------------------------------------------------------- */
/* Ensure capacity of results array.                          */
/* ----------------------------------------------------------- */
static void ensure_results_capacity(idx_t n)
{
    if (n + 1 <= results_cap) return;
    size_t new_cap = results_cap ? results_cap * 2 : 16;
    while (new_cap <= n) new_cap *= 2;
    seq_t *new_buf = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (results) {
        for (idx_t i = 1; i <= max_prepared_n; ++i) new_buf[i] = results[i];
        free(results);
    }
    results = new_buf;
    results_cap = new_cap;
}

/* ----------------------------------------------------------- */
/* Extend DP tables up to target_n.                            */
/* ----------------------------------------------------------- */
static void extend_to_n(idx_t target_n)
{
    if (target_n <= max_prepared_n) return;
    /* Need to process rows up to target_n + 2. */
    idx_t target_rows = target_n + 2;
    for (idx_t r = processed_rows + 1; r <= target_rows; ++r) {
        /* Zero dp_next */
        for (size_t i = 0; i < Nstates; ++i) dp_next[i] = 0;

        seq_t sum_total = 0;
        /* Transition: from state (a,b) to (b,c) */
        for (size_t s = 0; s < Nstates; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            unsigned char *dest_arr = trans_dest + s * V;
            unsigned char cnt = trans_outcnt[s];
            unsigned char b_idx = (unsigned char)(s % V);
            for (unsigned char i = 0; i < cnt; ++i) {
                unsigned char c_idx = dest_arr[i];
                size_t dest = (size_t)b_idx * V + c_idx;
                dp_next[dest] += val;
                sum_total += val;
            }
        }
        /* Store result for n = r - 2 */
        idx_t n_val = r - 2;
        ensure_results_capacity(n_val);
        results[n_val] = sum_total;

        /* Swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        processed_rows = r;
    }
    if (target_n > max_prepared_n) max_prepared_n = target_n;
}

/* ----------------------------------------------------------- */
/* Public sequence function.                                  */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_tables();
    if (n < 1) return 0;
    if (n > max_prepared_n) extend_to_n(n);
    return results[n];
}
