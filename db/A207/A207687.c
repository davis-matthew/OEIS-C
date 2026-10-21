/*
 * A207687  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 18, u64 n <= 41, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: DP for the number of 7×n binary arrays that avoid
 * horizontally the triples 000 and 101, and vertically the triples
 * 001 and 111.  The sequence starts at n = 1.
 *
 * This implementation builds all columns that satisfy the vertical
 * constraints, then uses a finite‑state automaton to add columns
 * while respecting horizontal constraints.  The DP is memoised in
 * static arrays so that calls in any order are supported.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Height of the arrays */
#define ROWS 7
/* All possible 7‑bit patterns (2^7 = 128) */
#define MAX_COLS ((1U << ROWS))

/* Forward declaration of the function required by the environment */
seq_t a(idx_t n);

/* ----------------------------------------------------------------
 * Static data used by the DP.  They are initialised exactly once.
 * ---------------------------------------------------------------- */

static bool initialized = false;

static int *col_bits;          /* bit pattern of each valid column */
static uint8_t *col_index;     /* mapping from pattern to its index (-1 if invalid) */
static int *transitions;       /* flattened transition table: state*V + k -> next state */
static uint16_t *trans_cnt;    /* number of transitions from each state */
static seq_t *dp_cur;          /* DP for the current column */
static seq_t *dp_nxt;          /* DP for the next column */

/* Number of valid columns and states */
static uint16_t V = 0;         /* number of vertically valid columns */
static uint32_t S = 0;         /* V * V, number of states */

/* ----------------------------------------------------------------
 * Helper that checks a column for the vertical constraints:
 * no triple of consecutive bits equals 001 or 111
 * ---------------------------------------------------------------- */
static bool is_vertical_ok(int bits)
{
    /* We only need to check rows 0..4 (inclusive) for a triple */
    for (int i = 0; i <= ROWS - 3; ++i) {
        int a = (bits >> i) & 1;
        int b = (bits >> (i + 1)) & 1;
        int c = (bits >> (i + 2)) & 1;
        if ((a == 0 && b == 0 && c == 1) || (a == 1 && b == 1 && c == 1))
            return false;
    }
    return true;
}

/* ----------------------------------------------------------------
 * Initialise tables once.  Called on the first call to a().
 * ---------------------------------------------------------------- */
static void init(void)
{
    if (initialized)
        return;

    /* Allocate arrays for column enumeration */
    col_bits   = (int *)malloc(MAX_COLS * sizeof(int));
    col_index  = (uint8_t *)malloc(MAX_COLS * sizeof(uint8_t));
    if (!col_bits || !col_index)
        return;    /* allocation failure – will be handled as 0 result */

    for (int i = 0; i < MAX_COLS; ++i)
        col_index[i] = 0xFF;   /* 0xFF indicates “invalid” */

    /* Find all bits patterns that satisfy the vertical constraints */
    V = 0;
    for (int bits = 0; bits < MAX_COLS; ++bits) {
        if (is_vertical_ok(bits)) {
            col_bits[V]   = bits;
            col_index[bits] = (uint8_t)V;
            ++V;
        }
    }

    /* Number of states = V * V (pairs of columns) */
    S = (uint32_t)V * (uint32_t)V;

    /* Allocate transition tables */
    transitions = (int *)malloc((size_t)S * (size_t)V * sizeof(int));
    trans_cnt   = (uint16_t *)malloc((size_t)S * sizeof(uint16_t));
    if (!transitions || !trans_cnt)
        return;  /* allocation failure – handled as 0 result */

    for (uint32_t i = 0; i < S; ++i)
        trans_cnt[i] = 0;

    /* Build transitions:
     * state (a_idx, b_idx) --> for each valid third column c_idx,
     * the triple (ar, br, cr) must not be 000 or 101 for any row.
     */
    for (int a_idx = 0; a_idx < V; ++a_idx) {
        int a = col_bits[a_idx];
        for (int b_idx = 0; b_idx < V; ++b_idx) {
            int b = col_bits[b_idx];
            uint32_t state = (uint32_t)a_idx * V + b_idx;
            for (int c_idx = 0; c_idx < V; ++c_idx) {
                int c = col_bits[c_idx];
                /* check horizontal constraints for all rows */
                bool ok = true;
                for (int r = 0; r < ROWS; ++r) {
                    int ar = (a >> r) & 1;
                    int br = (b >> r) & 1;
                    int cr = (c >> r) & 1;
                    if ((ar == 0 && br == 0 && cr == 0) ||
                        (ar == 1 && br == 0 && cr == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    /* transition to state (b_idx, c_idx) */
                    transitions[(size_t)state * V + trans_cnt[state]] =
                        (uint32_t)b_idx * V + c_idx;
                    ++trans_cnt[state];
                }
            }
        }
    }

    /* Allocate DP arrays */
    dp_cur = (seq_t *)calloc((size_t)S, sizeof(seq_t));
    dp_nxt = (seq_t *)calloc((size_t)S, sizeof(seq_t));

    initialized = true;
}

/* ----------------------------------------------------------------
 * Main function computing a(n).  n is 1‑based; n=0 returns 0.
 * ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();                     /* make sure tables are ready */

    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)V;
    if (n == 2) return (seq_t)V * (seq_t)V;

    /* Initialise DP for two columns: each state has count 1 */
    for (uint32_t s = 0; s < S; ++s)
        dp_cur[s] = (seq_t)1;

    /* Add columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        /* reset next DP array */
        for (uint32_t s = 0; s < S; ++s)
            dp_nxt[s] = (seq_t)0;

        /* perform transition */
        for (uint32_t s = 0; s < S; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0)
                continue;
            uint16_t cnt = trans_cnt[s];
            size_t base = (size_t)s * V;
            for (uint16_t k = 0; k < cnt; ++k) {
                uint32_t nxt = (uint32_t)transitions[base + k];
                dp_nxt[nxt] += val;
            }
        }
        /* swap pointers */
        seq_t *tmp   = dp_cur;
        dp_cur       = dp_nxt;
        dp_nxt       = tmp;
    }

    /* sum all final states */
    seq_t total = (seq_t)0;
    for (uint32_t s = 0; s < S; ++s)
        total += dp_cur[s];
    return total;
}
