/*
 * A239401  Number of nX4 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of elements above
 *          it, modulo 4.
 * https://oeis.org/A239401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays over {0,1,2,3} in which no entry
 *            equals 1 plus the sum of the entries to its left or 2 plus the
 *            sum of the entries above it (both sums taken modulo 4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t idx_t;

/* Transition entry: destination state and counting factor */
typedef struct {
    uint8_t to;            /* destination state encoded in 8 bits */
    int    cnt;            /* number of different assignments leading to this state */
} trans_entry;

/* Number of states: 4^4 = 256 */
#define N_STATES 256

/* Static tables used only once */
static trans_entry trans_table[N_STATES][N_STATES];
static int          trans_count[N_STATES] = {0};

/* Build the transition table lazily */
static void build_transitions(void) {
    for (int s = 0; s < N_STATES; ++s) {
        /* decode the old sum_above vector from state s */
        uint8_t old_above[4];
        for (int j = 0; j < 4; ++j) {
            old_above[j] = (s >> (2 * j)) & 3U;
        }

        /* temporary counter for each destination state */
        int cntd[N_STATES] = {0};

        /* Enumerate all 4^4 = 256 possible 4‑tuples */
        for (int a0 = 0; a0 < 4; ++a0)
            for (int a1 = 0; a1 < 4; ++a1)
                for (int a2 = 0; a2 < 4; ++a2)
                    for (int a3 = 0; a3 < 4; ++a3) {
                        int a[4] = {a0, a1, a2, a3};
                        int sum_left = 0;
                        bool ok = true;
                        uint8_t new_above[4];

                        for (int j = 0; j < 4; ++j) {
                            int forbid1 = (1 + sum_left) & 3;
                            int forbid2 = (2 + old_above[j]) & 3;
                            if (a[j] == forbid1 || a[j] == forbid2) {
                                ok = false;
                                break;
                            }
                            sum_left = (sum_left + a[j]) & 3;
                            new_above[j] = (old_above[j] + a[j]) & 3;
                        }

                        if (!ok) continue;

                        /* encode destination state */
                        int t = (new_above[0]) |
                                (new_above[1] << 2) |
                                (new_above[2] << 4) |
                                (new_above[3] << 6);
                        cntd[t] += 1;
                    }

        /* store non‑zero transitions */
        int idx = 0;
        for (int t = 0; t < N_STATES; ++t) {
            if (cntd[t] > 0) {
                trans_table[s][idx].to = (uint8_t)t;
                trans_table[s][idx].cnt = cntd[t];
                ++idx;
            }
        }
        trans_count[s] = idx;
    }
}

/* Main function required by the framework */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        build_transitions();
        initialized = true;
    }

    /* vector of counts for each state */
    seq_t cur[N_STATES] = {0};
    seq_t nxt[N_STATES] = {0};

    cur[0] = 1; /* before any rows, all sums_above are zero */

    for (idx_t step = 0; step < n; ++step) {
        /* zero next vector */
        for (int i = 0; i < N_STATES; ++i) nxt[i] = 0;

        for (int s = 0; s < N_STATES; ++s) {
            if (cur[s] == 0) continue;
            seq_t cur_val = cur[s];
            int tcnt = trans_count[s];
            for (int k = 0; k < tcnt; ++k) {
                int t = trans_table[s][k].to;
                int mult = trans_table[s][k].cnt;
                nxt[t] += cur_val * (seq_t)mult;
            }
        }

        /* swap cur and nxt */
        for (int i = 0; i < N_STATES; ++i) cur[i] = nxt[i];
    }

    /* sum over all states to get total number of arrays */
    seq_t total = 0;
    for (int i = 0; i < N_STATES; ++i) total += cur[i];
    return total;
}
