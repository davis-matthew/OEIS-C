/*
 * A207313  Number of nX4 0..2 arrays avoiding the pattern z-2 z-1 z in any
 *          row, column or nw-to-se diagonal.
 * https://oeis.org/A207313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A207313 – Number of n×4 0..2 arrays avoiding 0‑1‑2 in rows,
    columns or NW→SE diagonals.
    The values are computed by a transfer‑matrix DP on states
    (row i‑2 , row i‑1).  Each row is encoded as a 4‑digit base‑3
    number (0..80).  The routine keeps a static cache of all previously
    computed terms (for n up to 20000, enough for the tests). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define ROWS   81            /* 3^4 possible rows  */
#define STATES (ROWS*ROWS)   /* 81×81 state pairs  */
#define MAX_CACHE 20000      /* enough for tests   */

/* Per‑row data: digit decomposition and horizontal‑pattern flag. */
static uint8_t row_val[ROWS][4];
static bool    horiz_ok [ROWS];

/* For each state (prev_prev_row , prev_row) list all admissible next_rows
   that keep the three‑row window legal. */
static int transitions[STATES][ROWS];
static unsigned char trans_count[STATES];

/* DP arrays: number of ways to reach a given state after k rows. */
static seq_t dp_cur[STATES];
static seq_t dp_next[STATES];

/* Cache of sequence values: a[0] = a(1), a[1] = a(2), … */
static seq_t cache[MAX_CACHE];
static idx_t last_computed = 0;  /* number of terms stored (>=0) */

/* --------------------------------------------------------------------- */
/* Build table once – called lazily from a().                            */
static void init_tables(void)
{
    /* ----- row data ------------------------------------------------- */
    for (int num = 0; num < ROWS; ++num) {
        int x = num;
        for (int pos = 3; pos >= 0; --pos) {
            row_val[num][pos] = (uint8_t)(x % 3);
            x /= 3;
        }
        bool ok = true;
        /* horizontal 0‑1‑2 forbidden */
        for (int start = 0; start < 2; ++start) {
            if (row_val[num][start]     == 0 &&
                row_val[num][start + 1] == 1 &&
                row_val[num][start + 2] == 2) {
                ok = false;
                break;
            }
        }
        horiz_ok[num] = ok;
    }

    /* ----- transitions ------------------------------------------------ */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int state = r1 * ROWS + r2;
            if (!horiz_ok[r1] || !horiz_ok[r2]) {
                trans_count[state] = 0;
                continue;
            }
            int idx = 0;
            for (int r3 = 0; r3 < ROWS; ++r3) {
                if (!horiz_ok[r3]) continue;

                /* vertical 0‑1‑2 across the three rows */
                bool bad = false;
                for (int c = 0; c < 4; ++c) {
                    if (row_val[r1][c] == 0 &&
                        row_val[r2][c] == 1 &&
                        row_val[r3][c] == 2) {
                        bad = true;
                        break;
                    }
                }
                if (bad) continue;

                /* NW→SE diagonal 0‑1‑2 */
                if (row_val[r1][0] == 0 &&
                    row_val[r2][1] == 1 &&
                    row_val[r3][2] == 2) continue;
                if (row_val[r1][1] == 0 &&
                    row_val[r2][2] == 1 &&
                    row_val[r3][3] == 2) continue;

                transitions[state][idx++] = r2 * ROWS + r3;
            }
            trans_count[state] = (unsigned char)idx;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Main function required by the framework.                              */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* invalid offset – return 0 */

    /* Build tables on the first call. */
    static bool tables_inited = false;
    if (!tables_inited) {
        init_tables();
        tables_inited = true;
    }

    /* Return cached value if already computed. */
    if (last_computed >= n) {
        return cache[(size_t)(n - 1)];
    }

    /* Ensure cache large enough (MAX_CACHE is >10000 in tests). */
    if (n > MAX_CACHE) {
        /* According to the problem constraints this should not happen. */
        return 0;
    }

    /* Compute terms from (last_computed + 1) up to n. */
    for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
        seq_t sum = 0;

        /* ----------------------------------------------------------- */
        if (cur == 1) {
            /* a(1): count rows with no horizontal 0‑1‑2. */
            for (int r = 0; r < ROWS; ++r)
                if (horiz_ok[r]) sum += (seq_t)1;
            cache[0] = sum;
            last_computed = 1;
            continue;
        }

        if (cur == 2) {
            /* a(2): all ordered pairs of admissible rows. */
            memset(dp_cur, 0, sizeof(dp_cur));
            for (int r1 = 0; r1 < ROWS; ++r1) {
                if (!horiz_ok[r1]) continue;
                for (int r2 = 0; r2 < ROWS; ++r2) {
                    if (!horiz_ok[r2]) continue;
                    int state = r1 * ROWS + r2;
                    dp_cur[state] = (seq_t)1;
                    sum += (seq_t)1;
                }
            }
            cache[1] = sum;
            last_computed = 2;
            continue;
        }

        /* ----------------------------------------------------------- */
        /* cur >= 3 : transfer matrix step. */
        memset(dp_next, 0, sizeof(dp_next));

        for (int s = 0; s < STATES; ++s) {
            if (dp_cur[s] == (seq_t)0) continue;
            seq_t val = dp_cur[s];
            unsigned char cnt = trans_count[s];
            for (unsigned char k = 0; k < cnt; ++k) {
                int ns = transitions[s][k];
                dp_next[ns] += val;
            }
        }

        for (int i = 0; i < STATES; ++i) sum += dp_next[i];

        /* prepare for next iteration */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));

        cache[(size_t)(cur - 1)] = sum;
        last_computed = cur;
    }

    return cache[(size_t)(n - 1)];
}
