/*
 * A181257  Number of (n+2)X5 binary matrices with every 3X3 block having
 *          exactly four 1's.
 * https://oeis.org/A181257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×5 binary matrices with every 3×3 block
 *            having exactly four 1's. Computed by a transfer‑matrix (state
 *            DP) over pairs of consecutive rows
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- state / transition ------------------------------------------------ */

#define ROWS   32        /* 5‑bit rows */
#define STATES (ROWS*ROWS)

/* adjacency list: for each pair of rows (r1,r2) list of possible next rows r3 */
static int  adj[STATES][32];
static uint8_t adj_sz[STATES];

/* build the adjacency tables once */
static void build_adj(void)
{
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int u = r1*ROWS + r2;
            uint8_t sz = 0;
            for (int r3 = 0; r3 < ROWS; ++r3) {
                bool ok = true;
                for (int c = 0; c < 3; ++c) {
                    int s = 0;
                    for (int d = 0; d < 3; ++d) {
                        int bit1 = (r1 >> (4-(c+d))) & 1;
                        int bit2 = (r2 >> (4-(c+d))) & 1;
                        int bit3 = (r3 >> (4-(c+d))) & 1;
                        s += bit1 + bit2 + bit3;
                    }
                    if (s != 4) { ok = false; break; }
                }
                if (ok) {
                    int v = r2*ROWS + r3;
                    adj[u][sz++] = v;
                }
            }
            adj_sz[u] = sz;   /* up to 32 outgoing */
        }
    }
}

/* ---------- dynamic programming / memoisation -------------------------------- */

static seq_t dp_cur[STATES];  /* counts after current height (initially 2 rows) */
static seq_t dp_next[STATES];

static seq_t *totals = NULL;   /* a(1), a(2), … */
static idx_t tot_len = 0;      /* number of terms stored */
static idx_t tot_cap = 0;      /* allocated capacity */

static bool initialized = false;

/* ensure totals array large enough */
static void grow_totals(idx_t n)
{
    if (n < tot_cap) return;
    size_t new_cap = tot_cap ? tot_cap<<1 : 128;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    totals = (seq_t*)realloc(totals, new_cap * sizeof(seq_t));
    tot_cap = new_cap;
}

/* compute all terms up to index n (0‑based) */
static void compute_upto(idx_t n)
{
    if (n < tot_len) return;

    /* first time: initialise adjacency, dp_cur, and totals container */
    if (!initialized) {
        build_adj();
        for (int i = 0; i < STATES; ++i) dp_cur[i] = (seq_t)1; /* height 2 rows */
        tresa:
        ;
        initialize:
        ;
    }

    for (idx_t idx = tot_len; idx <= n; ++idx) {
        /* clear dp_next */
        for (int i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;

        /* one transition: use stored dp_cur (counts for current height) */
        for (int u = 0; u < STATES; ++u) {
            seq_t val = dp_cur[u];
            if (val == (seq_t)0) continue;
            uint8_t sz = adj_sz[u];
            for (uint8_t k = 0; k < sz; ++k) {
                int v = adj[u][k];
                dp_next[v] += val;
            }
        }

        /* sum all states to get a(n) */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < STATES; ++i) sum += dp_next[i];

        grow_totals(tot_len + 1);
        totals[tot_len++] = sum;

        /* swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur;
        for (int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }
    initialized = true;
}

/* ---------- public function ------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1 */
    compute_upto(n-1);                     /* ensure we have a(1)…a(n) */
    return totals[n-1];
}
