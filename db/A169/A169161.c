/*
 * A169161  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts reduced words of length n in the Coxeter group on 4
 *            generators S₀…S₃ with relations Sᵢ² = I and (SᵢSⱼ)²⁷ = I.
 *            Computed by dynamic programming on an automaton that tracks the
 *            last generator, its partner in the current alternating run, and
 *            the run length (max 53). This yields the enumer...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Maximum n expected in the test harness.  Increase if more values are needed. */
#ifndef MAX_N
#define MAX_N 10000
#endif

/* Relationship between partner indices and ranks for encoding states */
static const int partner[4][3] = {
    {1, 2, 3}, /* other letters when last = 0 */
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
};
static const int rank_of[4][4] = {
    {-1, 0, 1, 2},
    {0, -1, 0, 1},
    {1, 0, -1, 0},
    {2, 1, 0, -1}
};

#define MAX_RUN 53
#define NUM_STATES (4 + 4 * 3 * (MAX_RUN - 1))   /* 628 states */

/* Encode a state (last, partner, run) into an array index */
static inline size_t state_index(int last, int partner_letter, int run)
{
    if (run == 1) {
        return (size_t)last;                 /* states 0–3 */
    }
    int rnk = rank_of[last][partner_letter];
    return (size_t)4 + ((last * 3 + rnk) * (MAX_RUN - 1) + (run - 2));
}

/* External sequence function required by the benchmark harness */
seq_t a(idx_t n)
{
    /* Cache of already computed values */
    static idx_t computed_len = 0;              /* highest index for which values[] is valid */
    static seq_t values[MAX_N + 1] = {0};

    /* Initialisation of base case */
    if (computed_len == 0) {
        values[0] = (seq_t)1;
        computed_len = 0;
    }

    if (n <= computed_len) return values[n];

    /* Transition tables for the automaton */
    static seq_t state_arr[NUM_STATES];
    static seq_t next_arr[NUM_STATES];
    static seq_t *state_counts = state_arr;
    static seq_t *next_counts  = next_arr;
    static int dp_initialised = 0;

    if (!dp_initialised) {
        for (size_t i = 0; i < NUM_STATES; ++i) state_counts[i] = 0;
        for (size_t i = 0; i < NUM_STATES; ++i) next_counts[i]  = 0;
        dp_initialised = 1;
    }

    /* Extend the sequence up to n */
    while (computed_len < n) {
        /* Zero next_counts for the new level */
        for (size_t i = 0; i < NUM_STATES; ++i) next_counts[i] = 0;

        if (computed_len == 0) {
            /* First letter – transitions from the empty word */
            for (int L = 0; L < 4; ++L) {
                size_t idx = state_index(L, L, 1);
                next_counts[idx] += (seq_t)1;
            }
        } else {
            /* General DP step */
            for (size_t s = 0; s < NUM_STATES; ++s) {
                seq_t cnt = state_counts[s];
                if (cnt == 0) continue;

                int last, partner_letter, run;
                if (s < 4) {
                    /* Run length 1 state – partner is irrelevant */
                    last = (int)s;
                    partner_letter = last;
                    run = 1;
                } else {
                    size_t t = s - 4;
                    last = (int)(t / (3 * (MAX_RUN - 1)));
                    size_t rem = t % (3 * (MAX_RUN - 1));
                    int rnk = (int)(rem / (MAX_RUN - 1));
                    run = (int)(rem % (MAX_RUN - 1)) + 2;
                    partner_letter = partner[last][rnk];
                }

                for (int next = 0; next < 4; ++next) {
                    if (next == last) continue;          /* Sᵢ² = I */

                    if (next == partner_letter) {
                        if (run == MAX_RUN) continue;    /* would reach forbidden length 54 */
                        int new_last = next;
                        int new_partner = last;
                        int new_run = run + 1;
                        size_t idx = state_index(new_last, new_partner, new_run);
                        next_counts[idx] += cnt;
                    } else {
                        int new_last = next;
                        int new_partner = last;
                        int new_run = 1;
                        size_t idx = state_index(new_last, new_partner, new_run);
                        next_counts[idx] += cnt;
                    }
                }
            }
        }

        /* Sum all counts for the new length */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < NUM_STATES; ++i) total += next_counts[i];

        ++computed_len;
        if (computed_len <= MAX_N) values[computed_len] = total;

        /* Rotate buffers for the next iteration */
        seq_t *tmp = state_counts;
        state_counts = next_counts;
        next_counts = tmp;
    }

    return values[n];
}
