/*
 * A227667  Number of lattice paths from {n}^5 to {0}^5 using steps that
 *          decrement one component by 1 such that for each point
 *          (p_1,p_2,...,p_5) we have abs(p_{i}-p_{i+1}) <= 1.
 * https://oeis.org/A227667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of lattice paths in 5‑D from (n,…,n) to (0,…,0)
 *            with steps decreasing one coordinate by 1 and |p_i – p_{i+1}| ≤
 *            1. For n ≥ 9 the values satisfy the 9‑term linear recurrence
 *            a(n) = 290 a(n−1) − 9016 a(n−2) − 23648 a(n−3) + 5084576 a(n−4)
 *            − 3950640 a(n−5) + 37771968 a(n−6) − 36900032...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   abs8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/abs8.h"

/* ----------  Helper to compute a(n) for n ≤ 8 by exhaustive DP  ---------- */
static inline uint32_t encode_state(const uint8_t p[5])
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 4) |
           ((uint32_t)p[2] << 8) | ((uint32_t)p[3] << 12) |
           ((uint32_t)p[4] << 16);
}

/* Compute a(n) for n <= 8 using brute-force DP over all admissible states. */
static seq_t compute_small_a(idx_t n)
{
    /* Max number of admissible 5‑tuples for n = 8 is 9^5 = 59049.  */
    const int MAX_TUPLES = 59049;
    struct State {
        uint8_t p[5];
        idx_t   sum;
        seq_t   count;
    } states[MAX_TUPLES];
    int total = 0;
    idx_t N = (idx_t)n;

    /* Enumerate all 5‑tuples with |p_i – p_{i+1}| ≤ 1 and 0 ≤ p_i ≤ N. */
    for (uint8_t a = 0; a <= N; ++a)
        for (uint8_t b = 0; b <= N; ++b)
            if (abs8(a, b) <= 1)
                for (uint8_t c = 0; c <= N; ++c)
                    if (abs8(b, c) <= 1)
                        for (uint8_t d = 0; d <= N; ++d)
                            if (abs8(c, d) <= 1)
                                for (uint8_t e = 0; e <= N; ++e)
                                    if (abs8(d, e) <= 1) {
                                        states[total].p[0] = a;
                                        states[total].p[1] = b;
                                        states[total].p[2] = c;
                                        states[total].p[3] = d;
                                        states[total].p[4] = e;
                                        states[total].sum = (idx_t)(a + b + c + d + e);
                                        states[total].count = 0;
                                        ++total;
                                    }

    /* Map from encoded state to index in states array. */
    static int index_map[1 << 20];
    memset(index_map, -1, sizeof(index_map));

    int startIndex = -1;
    for (int i = 0; i < total; ++i) {
        uint32_t key = encode_state(states[i].p);
        index_map[key] = i;
        if (states[i].p[0] == N && states[i].p[1] == N &&
            states[i].p[2] == N && states[i].p[3] == N &&
            states[i].p[4] == N)
            startIndex = i;
    }
    /* Count the zero state. */
    int zeroIndex = index_map[0];
    states[zeroIndex].count = 1;

    /* Propagate counts from lower sums to higher sums. */
    idx_t maxSum = 5 * N;
    for (idx_t s = 0; s <= maxSum; ++s) {
        for (int i = 0; i < total; ++i) {
            if (states[i].sum != s || states[i].count == 0)
                continue;
            for (int comp = 0; comp < 5; ++comp) {
                if (states[i].p[comp] == N)
                    continue; /* cannot increase beyond N in this direction */
                uint8_t nextP[5];
                memcpy(nextP, states[i].p, 5);
                ++nextP[comp];
                /* Check the adjacency constraint for the new state. */
                bool ok = true;
                for (int j = 0; j < 4; ++j)
                    if (abs8(nextP[j], nextP[j + 1]) > 1) {
                        ok = false;
                        break;
                    }
                if (!ok)
                    continue;
                uint32_t key = encode_state(nextP);
                int idx = index_map[key];
                if (idx < 0)
                    continue; /* should not happen */
                states[idx].count += states[i].count;
            }
        }
    }
    return states[startIndex].count;
}

/* ----------  Main sequence function  ---------- */
static seq_t init_vals[9];
static bool init_done = false;

/* Ensure initial values for n = 0 … 8 are computed once. */
static void ensure_initial_values(void)
{
    if (init_done)
        return;
    for (int i = 0; i < 9; ++i)
        init_vals[i] = compute_small_a((idx_t)i);
    init_done = true;
}

/* Recurrence coefficients as seq_t constants.  */
#define C290            ((seq_t)290ULL)
#define C9016           ((seq_t)9016ULL)
#define C23648          ((seq_t)23648ULL)
#define C5084576        ((seq_t)5084576ULL)
#define C3950640        ((seq_t)3950640ULL)
#define C37771968       ((seq_t)37771968ULL)
#define C36900032       ((seq_t)36900032ULL)
#define C4024832        ((seq_t)4024832ULL)
#define C98304          ((seq_t)98304ULL)

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_initial_values();

    if (n < 9ULL)
        return init_vals[(int)n];

    /* Buffer holding a(k-1) … a(k-9) for the recurrence.  */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i)
        buf[i] = init_vals[8 - i];   /* buf[0] = a(8), …, buf[8] = a(0) */

    for (idx_t k = 9; k <= n; ++k) {
        seq_t pos = buf[0] * C290
                  + buf[3] * C5084576
                  + buf[5] * C37771968
                  + buf[7] * C4024832;
        seq_t neg = buf[1] * C9016
                  + buf[2] * C23648
                  + buf[4] * C3950640
                  + buf[6] * C36900032
                  + buf[8] * C98304;
        seq_t val = pos - neg;   /* result is non‑negative for all n.  */

        /* Shift the ring buffer left by one position. */
        for (int j = 8; j > 0; --j)
            buf[j] = buf[j - 1];
        buf[0] = val;
    }
    return buf[0];
}
