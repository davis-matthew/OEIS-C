/*
 * A240372  Number of nX3 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it or zero plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays of entries 0..3 with no element
 *            equal to (1 + sum of elements to its left) mod 4, (1 + sum of
 *            elements above it) mod 4, or (sum of elements diagonally
 *            northwest of it) mod 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint16_t trans[4096][64];
static unsigned char trans_len[4096];
static bool trans_built = false;

/* Encode six 2‑bit values into a 12‑bit state index. */
static inline unsigned encode_state(unsigned c0, unsigned c1, unsigned c2,
                                    unsigned d0, unsigned d1, unsigned d2)
{
    return (c0 << 10) | (c1 << 8) | (c2 << 6) | (d0 << 4) | (d1 << 2) | d2;
}

/* Build the transition table once. */
static void build_transitions(void)
{
    for (unsigned s = 0; s < 4096; ++s) {
        unsigned c0 = (s >> 10) & 3;
        unsigned c1 = (s >> 8) & 3;
        unsigned c2 = (s >> 6) & 3;
        unsigned d0 = (s >> 4) & 3;
        unsigned d1 = (s >> 2) & 3;
        unsigned d2 = s & 3;
        unsigned char len = 0;

        for (unsigned q0 = 0; q0 < 4; ++q0) {
            unsigned forb0_1 = 1;                      /* 1 + 0 mod 4 */
            unsigned forb0_2 = (1 + c0) & 3;           /* 1 + sum above (c0) */
            unsigned forb0_3 = d2 & 3;                 /* sum diagonal (d2) */
            if (q0 == forb0_1 || q0 == forb0_2 || q0 == forb0_3)
                continue;

            for (unsigned q1 = 0; q1 < 4; ++q1) {
                unsigned forb1_1 = (1 + q0) & 3;
                unsigned forb1_2 = (1 + c1) & 3;
                unsigned forb1_3 = d1 & 3;
                if (q1 == forb1_1 || q1 == forb1_2 || q1 == forb1_3)
                    continue;

                for (unsigned q2 = 0; q2 < 4; ++q2) {
                    unsigned left_sum = (q0 + q1) & 3;
                    unsigned forb2_1 = (1 + left_sum) & 3;
                    unsigned forb2_2 = (1 + c2) & 3;
                    unsigned forb2_3 = d0 & 3;
                    if (q2 == forb2_1 || q2 == forb2_2 || q2 == forb2_3)
                        continue;

                    /* Compute next state after adding this row. */
                    unsigned nc0 = (c0 + q0) & 3;
                    unsigned nc1 = (c1 + q1) & 3;
                    unsigned nc2 = (c2 + q2) & 3;
                    unsigned nd0 = (d1 + q1) & 3;
                    unsigned nd1 = (d2 + q0) & 3;
                    unsigned nd2 = 0;                      /* new diagonal */

                    unsigned next = encode_state(nc0, nc1, nc2, nd0, nd1, nd2);
                    trans[s][len++] = (uint16_t)next;
                }
            }
        }
        trans_len[s] = len;
    }
    trans_built = true;
}

/* Main sequence function used by the test harness. */
seq_t a(idx_t n)
{
    if (!trans_built)
        build_transitions();

    static seq_t counts[4096];
    static seq_t new_counts[4096];
    static bool visited[4096];
    static int active[4096];
    static int new_active[4096];
    static int active_count = 0;
    static int new_active_count = 0;

    /* Reset state for each call. */
    memset(counts, 0, sizeof(counts));
    counts[0] = 1;                 /* starting state before any rows */
    active[0] = 0;
    active_count = 1;

    for (idx_t step = 0; step < n; ++step) {
        memset(new_counts, 0, sizeof(new_counts));
        memset(visited, 0, sizeof(visited));
        new_active_count = 0;

        for (int ai = 0; ai < active_count; ++ai) {
            int s = active[ai];
            seq_t cs = counts[s];
            if (cs == 0)
                continue;

            unsigned char len = trans_len[s];
            for (unsigned char k = 0; k < len; ++k) {
                unsigned short ns = trans[s][k];
                if (!visited[ns]) {
                    visited[ns] = true;
                    new_active[new_active_count++] = ns;
                }
                new_counts[ns] += cs;
            }
        }

        /* Prepare for next iteration. */
        memcpy(counts, new_counts, sizeof(counts));
        memcpy(active, new_active, new_active_count * sizeof(int));
        active_count = new_active_count;
    }

    /* Sum all counts to obtain a(n). */
    seq_t result = 0;
    for (int ai = 0; ai < active_count; ++ai)
        result += counts[active[ai]];
    return result;
}
