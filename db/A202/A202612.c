/*
 * A202612  Number of (n+2)X6 binary arrays avoiding patterns 001 and 010 in
 *          rows and columns.
 * https://oeis.org/A202612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A202612
   Number of (n+2)×6 binary arrays avoiding patterns 001 and 010
   in rows and columns.  A transfer‑matrix DP over the last
   two rows is used.  All arithmetic is performed with seq_t,
   the arbitrary‑precision integer type supplied by the build. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Size of the state space: last two rows, each 6 bits. */
#define STATE_BITS 12
#define STATE_SIZE (1U << STATE_BITS)

/* Maximum number of valid row patterns (length‑6 words avoiding
   001 and 010).  In fact it is 21. */
#define MAX_ROWS 64

/* Lookup table for adjacency.  For a given state (a,b)
   adj_next[s][k] = (b,c) for all permissible next rows c.   */
static int adj_next[STATE_SIZE][MAX_ROWS];
static int adj_len[STATE_SIZE];

/* List of valid 6‑bit rows.   */
static uint8_t valid_rows[MAX_ROWS];
static int R = 0;

/* One‑time construction of tables.  */
static bool tables_ready = false;

static void init_tables(void)
{
    if (tables_ready) return;

    /* Build list of all 6‑bit rows that avoid 001 and 010 horizontally. */
    for (int p = 0; p < 64; ++p) {
        bool ok = true;
        for (int i = 0; i <= 3; ++i) {
            int w = (p >> i) & 0x7;          /* 3‑bit window */
            if (w == 0b001 || w == 0b010) { /* forbidden */
                ok = false;
                break;
            }
        }
        if (ok) {
            valid_rows[R++] = (uint8_t)p;
        }
    }

    /* Pre‑compute adjacency list for the transfer matrix. */
    for (int s = 0; s < STATE_SIZE; ++s) {
        /* Decode rows a (high 6 bits) and b (low 6 bits). */
        int a_row = (s >> 6) & 0x3F;
        int b_row =  s        & 0x3F;
        int len = 0;

        for (int idx = 0; idx < R; ++idx) {
            int c_row = valid_rows[idx];
            bool bad = false;
            for (int col = 0; col < 6; ++col) {
                int a_bit = (a_row >> col) & 1;
                int b_bit = (b_row >> col) & 1;
                int c_bit = (c_row >> col) & 1;
                int w = (a_bit << 2) | (b_bit << 1) | c_bit;
                if (w == 0b001 || w == 0b010) {
                    bad = true;
                    break;
                }
            }
            if (!bad) {
                int next_state = (b_row << 6) | c_row;
                adj_next[s][len++] = next_state;
            }
        }
        adj_len[s] = len;
    }

    tables_ready = true;
}

seq_t a(idx_t n)
{
    /* Height of the array is n+2 rows.  For the OEIS offset 1,
       n=1 corresponds to 3 rows.  For n=0 no arrays are counted. */
    if (n == 0) return (seq_t)0;

    init_tables();

    idx_t steps = n;                 /* number of additional rows after the first two */

    /* Temporary DP vectors per call.  Their size is fixed (4096). */
    seq_t dp[STATE_SIZE];
    seq_t dp_next[STATE_SIZE];

    /* Initialize local arrays to zero. */
    for (idx_t i = 0; i < STATE_SIZE; ++i) {
        dp[i]     = (seq_t)0;
        dp_next[i] = (seq_t)0;
    }

    /* Initial state: any ordered pair of valid rows.  Each pair contributes 1. */
    for (int i = 0; i < R; ++i) {
        uint8_t a_row = valid_rows[i];
        for (int j = 0; j < R; ++j) {
            uint8_t b_row = valid_rows[j];
            int state = (a_row << 6) | b_row;
            dp[state] = dp[state] + (seq_t)1;
        }
    }

    /* Transition steps. */
    for (idx_t step = 0; step < steps; ++step) {
        /* Zero dp_next. */
        for (idx_t i = 0; i < STATE_SIZE; ++i) dp_next[i] = (seq_t)0;

        /* Apply transition for every state with non‑zero count. */
        for (int s = 0; s < STATE_SIZE; ++s) {
            seq_t cnt = dp[s];
            if (!cnt) continue;
            int len = adj_len[s];
            int *list = adj_next[s];
            for (int k = 0; k < len; ++k) {
                int ns = list[k];
                dp_next[ns] = dp_next[ns] + cnt;
            }
        }

        /* Swap dp and dp_next for the next iteration. */
        for (int i = 0; i < STATE_SIZE; ++i) dp[i] = dp_next[i];
    }

    /* Sum all final states to obtain the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_SIZE; ++i) {
        total = total + dp[i];
    }

    return total;
}
