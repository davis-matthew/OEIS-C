/*
 * A224375  Number of 3Xn 0..2 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A224375 – Number of 3×n arrays with entries 0..2, rows unimodal
 *            and anti‑diagonals nondecreasing. Computed by dynamic
 *            programming over column states. Each state stores the three cell
 *            values (0..2) and for each row a phase flag: 0 = still
 *            non‑decreasing, 1 = already decreasing. Transitions check u...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 21, u64 n <= 159, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

enum {
    NUM_ROWS = 3,
    VALUES   = 3,
    PHASES   = 3,
    STATE_NUM = VALUES * VALUES * VALUES * PHASES * PHASES * PHASES   /* 3^3 * 2^3 = 216 */
};

/* State consisting of 3 values (0..2) and 3 phase flags (0 or 1). */
typedef struct {
    uint8_t v[NUM_ROWS];   /* cell values */
    uint8_t p[NUM_ROWS];   /* phase flags */
} State;

static State states[STATE_NUM];
static unsigned char compat_cnt[STATE_NUM];
static int compat[STATE_NUM][STATE_NUM];   /* adjacency list per state */

/* Build all states and their adjacency lists once. */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Enumerate all 216 states. */
    int idx = 0;
    for (int v0 = 0; v0 < VALUES; ++v0)
        for (int v1 = 0; v1 < VALUES; ++v1)
            for (int v2 = 0; v2 < VALUES; ++v2)
                for (int p0 = 0; p0 < PHASES; ++p0)
                    for (int p1 = 0; p1 < PHASES; ++p1)
                        for (int p2 = 0; p2 < PHASES; ++p2) {
                            states[idx].v[0] = (uint8_t)v0;
                            states[idx].v[1] = (uint8_t)v1;
                            states[idx].v[2] = (uint8_t)v2;
                            states[idx].p[0] = (uint8_t)p0;
                            states[idx].p[1] = (uint8_t)p1;
                            states[idx].p[2] = (uint8_t)p2;
                            ++idx;
                        }

    /* Compute compatibility lists. */
    for (int i = 0; i < STATE_NUM; ++i) {
        compat_cnt[i] = 0;
        for (int j = 0; j < STATE_NUM; ++j) {
            bool ok = true;

            /* Unimodality and phase update per row. */
            for (int r = 0; r < NUM_ROWS && ok; ++r) {
                int vprev = states[i].v[r];
                int vcurr = states[j].v[r];
                int pprev = states[i].p[r];
                int pcurr = states[j].p[r];

                if (pprev == 0) {            /* still non‑decreasing */
                    if (vcurr < vprev) {
                        if (pcurr != 1) ok = false;   /* switch to decreasing */
                    } else { /* vcurr >= vprev */
                        if (vcurr > vprev) {
                            if (pcurr != 0) ok = false;
                        } else { /* equal */
                            if (pcurr != 0) ok = false;
                        }
                    }
                } else {                  /* already decreasing */
                    if (vcurr > vprev) ok = false;
                    if (pcurr != 1) ok = false;
                }
            }

            /* Anti‑diagonal constraints: new column compared to previous. */
            if (states[j].v[0] > states[i].v[1]) ok = false;
            if (states[j].v[1] > states[i].v[2]) ok = false;

            if (ok) {
                compat[i][compat_cnt[i]++] = j;
            }
        }
    }
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    init();

    if (n == 1) return (seq_t)27;            /* 3^3 possible first columns */

    seq_t dp[STATE_NUM];
    seq_t dp_next[STATE_NUM];
    /* Initialize first column states – all phases zero. */
    for (int i = 0; i < STATE_NUM; ++i)
        dp[i] = (states[i].p[0] == 0 &&
                 states[i].p[1] == 0 &&
                 states[i].p[2] == 0) ? (seq_t)1 : (seq_t)0;

    /* Process remaining columns. */
    for (idx_t step = 1; step < n; ++step) {
        /* zero dp_next */
        for (int i = 0; i < STATE_NUM; ++i) dp_next[i] = (seq_t)0;

        for (int i = 0; i < STATE_NUM; ++i) {
            seq_t cnt = dp[i];
            if (cnt == (seq_t)0) continue;
            unsigned char kmax = compat_cnt[i];
            for (int k = 0; k < kmax; ++k) {
                int j = compat[i][k];
                dp_next[j] += cnt;     /* modulo seq_t arithmetic */
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < STATE_NUM; ++i) dp[i] = dp_next[i];
    }

    /* Sum all final states. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATE_NUM; ++i) result += dp[i];
    return result;
}
