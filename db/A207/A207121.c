/*
 * A207121  Number of nX6 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 21, u64 n <= 195, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of OEIS A207121:
    Number of n×6 binary arrays avoiding
    horizontally 001,010 and vertically 001,011.
    The algorithm enumerates all legal 6‑bit rows and
    dynamic‑programs over the last two rows.
    Initial values: a(1)=21, a(2)=441.
    (Implemented in C11/C++17; uses only integer operations.) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* tables for the 6‑bit rows that satisfy the horizontal constraints */
static int valid_rows[64];      /* all legal rows (0…63)                */
static int rows_cnt;           /* number of legal rows (21)            */

/* transition tables: for each pair (i,j) we store all k such that
   rows i,j,k obey the vertical constraints.
   Number of possible states: rows_cnt * rows_cnt = 441.          */
static int next_cnt[512];              /* number of transitions</int>          */
static int next_state[512][21];        /* index of next state (j,k)           */

/* DP buffers (size 512 > 441).  They are swapped by pointer. */
static seq_t dp_buf[2][512];

/* ---------------  initialization ----------------------------------*/
static void init_tables(void)
{
    int i, j, k, col, valid, bits0, bits1, bits2, triple;
    int state, cnt;

    /* clear mapping */
    for (i = 0; i < 64; ++i)
        valid_rows[i] = 0;

    rows_cnt = 0;
    for (i = 0; i < 64; ++i) {
        /* check horizontal patterns 001,010 in the 6‑bit row */
        valid = 1;
        for (col = 0; col <= 3; ++col) {
            int tri = (i >> col) & 0x7;   /* 3 consecutive bits */
            if (tri == 0b001 || tri == 0b010) {
                valid = 0;
                break;
            }
        }
        if (valid) {
            valid_rows[rows_cnt] = i;
            rows_cnt++;
        }
    }

    /* transitions */
    for (i = 0; i < rows_cnt; ++i) {
        for (j = 0; j < rows_cnt; ++j) {
            state = i * rows_cnt + j;
            cnt = 0;
            for (k = 0; k < rows_cnt; ++k) {
                int good = 1;
                for (col = 0; col < 6; ++col) {
                    bits0 = (valid_rows[i] >> col) & 1;   /* row i */
                    bits1 = (valid_rows[j] >> col) & 1;   /* row j */
                    bits2 = (valid_rows[k] >> col) & 1;   /* row k */
                    triple = (bits0 << 2) | (bits1 << 1) | bits2;
                    if (triple == 0b001 || triple == 0b011) {
                        good = 0;
                        break;
                    }
                }
                if (good) {
                    next_state[state][cnt] = j * rows_cnt + k;
                    cnt++;
                }
            }
            next_cnt[state] = cnt;
        }
    }
}

/* --------------  sequence function --------------------------------*/
seq_t a(idx_t n)
{
    /* lazy initialization */
    static int initialized = 0;
    if (!initialized) {
        init_tables();
        initialized = 1;
    }

    /* base cases */
    if (n == 0) return (seq_t)0;                      /* outside offset */
    if (n == 1) return (seq_t)rows_cnt;                /* 21 */
    if (n == 2) return (seq_t)rows_cnt * (seq_t)rows_cnt; /* 441 */

    const size_t total_states = (size_t)rows_cnt * (size_t)rows_cnt; /* 441 */
    seq_t *cur = dp_buf[0];
    seq_t *nxt = dp_buf[1];

    /* initialise counts for all pairs of rows: each state gets 1 */
    for (size_t s = 0; s < total_states; ++s)
        cur[s] = (seq_t)1;

    /* number of further rows to add after the first two */
    size_t steps = (size_t)(n - 2);
    for (size_t step = 0; step < steps; ++step) {
        /* zero next buffer */
        for (size_t s = 0; s < total_states; ++s)
            nxt[s] = (seq_t)0;

        /* transition */
        for (size_t state = 0; state < total_states; ++state) {
            seq_t val = cur[state];
            if (val == (seq_t)0) continue;
            int cnt = next_cnt[state];
            for (int t = 0; t < cnt; ++t) {
                int nextIdx = next_state[state][t];
                nxt[nextIdx] += val;
            }
        }

        /* swap pointers */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all counts for final state */
    seq_t result = (seq_t)0;
    for (size_t s = 0; s < total_states; ++s)
        result += cur[s];

    return result;
}
