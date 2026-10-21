/*
 * A234147  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of the edge differences
 *          equal to 10 and no adjacent elements equal.
 * https://oeis.org/A234147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The implementation counts a(n) by a transfer‑matrix DP for (n+1)
 *            × 3 arrays over {0,…,5} that have no equal adjacent entries and
 *            for which each 2×2 subblock has the sum of absolute edge
 *            differences equal to 10
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_STATES 150   /* number of admissible rows (3 columns) */
#define MAX_TRANS  150   /* maximum outgoing transitions per state */

typedef struct {
    int  vals[3];          /* values in the row: a,b,c */
    unsigned int deg;      /* number of transitions out of this state */
    int        nxt[MAX_TRANS];  /* indices of target states */
} state_t;

static state_t state[MAX_STATES];
static bool init_done = false;

/* Precompute all states and transitions. */
static void init_states(void)
{
    if (init_done) return;

    size_t idx = 0;
    /* enumerate all rows with adjacent unequal entries */
    for (int a = 0; a <= 5; ++a)
        for (int b = 0; b <= 5; ++b)
            if (b != a)
                for (int c = 0; c <= 5; ++c)
                    if (c != b) {
                        state[idx].vals[0] = a;
                        state[idx].vals[1] = b;
                        state[idx].vals[2] = c;
                        state[idx].deg = 0;
                        ++idx;
                    }

    /* precompute transitions */
    for (size_t i = 0; i < MAX_STATES; ++i) {
        int a = state[i].vals[0];
        int b = state[i].vals[1];
        int c = state[i].vals[2];
        for (size_t j = 0; j < MAX_STATES; ++j) {
            int x = state[j].vals[0];
            int y = state[j].vals[1];
            int z = state[j].vals[2];

            /* vertical equalities prohibited */
            if (a == x || b == y || c == z) continue;

            /* sum for left 2×2 block: |a−b| + |x−y| + |a−x| + |b−y| */
            int sb1 = (a > b ? a - b : b - a)
                    + (x > y ? x - y : y - x)
                    + (a > x ? a - x : x - a)
                    + (b > y ? b - y : y - b);
            if (sb1 != 10) continue;

            /* sum for right 2×2 block: |b−c| + |y−z| + |b−y| + |c−z| */
            int sb2 = (b > c ? b - c : c - b)
                    + (y > z ? y - z : z - y)
                    + (b > y ? b - y : y - b)
                    + (c > z ? c - z : z - c);
            if (sb2 != 10) continue;

            /* admissible transition */
            state[i].nxt[state[i].deg++] = (int)j;
        }
    }

    init_done = true;
}

/* ===================================================================== */
/* Public sequence function                                               */
/* ===================================================================== */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 */

    init_states();

    /* DP arrays */
    static seq_t cur[MAX_STATES];
    static seq_t nxt[MAX_STATES];

    size_t s;

    /* initial row: one way for each state */
    for (s = 0; s < MAX_STATES; ++s) cur[s] = 1;

    /* perform n transitions (n+1 rows → n steps) */
    for (idx_t step = 0; step < n; ++step) {
        for (s = 0; s < MAX_STATES; ++s) nxt[s] = 0;
        for (size_t i = 0; i < MAX_STATES; ++i) {
            seq_t ways = cur[i];
            if (ways == 0) continue;
            for (size_t k = 0; k < state[i].deg; ++k) {
                int j = state[i].nxt[k];
                nxt[j] += ways;
            }
        }
        /* copy nxt into cur for next step */
        for (s = 0; s < MAX_STATES; ++s) cur[s] = nxt[s];
    }

    /* sum over all possible last rows */
    seq_t total = 0;
    for (s = 0; s < MAX_STATES; ++s) total += cur[s];
    return total;
}
