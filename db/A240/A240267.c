/*
 * A240267  Number of nX3 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it or one plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2,3} such that for each
 *            cell the value is not equal to (sum of the elements to its left)
 *            (mod 4) , (sum of the elements above it) + 1 (mod 4) , (sum of
 *            the elements diagonally to its northwest) + 1 (mod 4). The
 *            sequence starts with a(1)=10
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pack_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pack_state.h"

/* Number of states: 3 columns for column sums, 2 columns for previous row,
   and one column for the row before that. */
#define STATE_COUNT 4096

typedef struct {
    unsigned int *items;
    unsigned int cnt;
} edge_list;

/* Pre‑compute the transition lists once */
static edge_list *transitions = NULL;

/* Called once before the first computation.  */
static void init_transitions(void)
{
    if (transitions) return;        /* already built */
    transitions = (edge_list *)malloc(sizeof(edge_list) * STATE_COUNT);
    for (unsigned int s = 0; s < STATE_COUNT; ++s) {
        transitions[s].items = NULL;
        transitions[s].cnt  = 0;
    }

    /* For each state, enumerate all 64 possible rows (r0,r1,r2) and
       add a transition if the row satisfies all constraints.           */
    for (unsigned int cs0 = 0; cs0 < 4; ++cs0)
    for (unsigned int cs1 = 0; cs1 < 4; ++cs1)
    for (unsigned int cs2 = 0; cs2 < 4; ++cs2)
    for (unsigned int pr0 = 0; pr0 < 4; ++pr0)
    for (unsigned int pr1 = 0; pr1 < 4; ++pr1)
    for (unsigned int pp0 = 0; pp0 < 4; ++pp0)
    {
        unsigned int state = pack_state(cs0,cs1,cs2, pr0,pr1,pp0);
        /* Temporary array large enough for all possible rows. */
        unsigned int tmp[64];
        unsigned int tcnt = 0;

        /* Try all 64 rows.  */
        for (unsigned int r0 = 0; r0 < 4; ++r0) {
            /* Check column 0 constraints. */
            unsigned int f2 = (cs0 + 1) & 3;
            if (r0 == 0 || r0 == f2 || r0 == 1) continue;   /* 0 (left), f2, 1 (diag+1) */

            for (unsigned int r1 = 0; r1 < 4; ++r1) {
                /* Check column 1 constraints. */
                unsigned int f2_c1 = (cs1 + 1) & 3;
                unsigned int f3_c1 = (pr0 + 1) & 3;
                if (r1 == r0 || r1 == f2_c1 || r1 == f3_c1) continue;

                for (unsigned int r2 = 0; r2 < 4; ++r2) {
                    /* Check column 2 constraints. */
                    unsigned int left = (r0 + r1) & 3;
                    unsigned int f2_c2 = (cs2 + 1) & 3;
                    unsigned int f3_c2 = (pr1 + pp0 + 1) & 3;
                    if (r2 == left || r2 == f2_c2 || r2 == f3_c2) continue;

                    /* Valid row found: compute the new state. */
                    unsigned int ncs0 = (cs0 + r0) & 3;
                    unsigned int ncs1 = (cs1 + r1) & 3;
                    unsigned int ncs2 = (cs2 + r2) & 3;
                    unsigned int npr0 = r0;
                    unsigned int npr1 = r1;
                    unsigned int npp0 = pr0;           /* row i-1 becomes row i-2 for next step */
                    tmp[tcnt++] = pack_state(ncs0,ncs1,ncs2, npr0,npr1,npp0);
                }
            }
        }
        /* Allocate the transition list for this state. */
        transitions[state].cnt = tcnt;
        if (tcnt) {
            transitions[state].items = (unsigned int *)malloc(tcnt * sizeof(unsigned int));
            for (unsigned int i = 0; i < tcnt; ++i) transitions[state].items[i] = tmp[i];
        }
    }
}

/* Free the allocated transition tables (optional). */
static void free_transitions(void)
{
    if (!transitions) return;
    for (unsigned int s = 0; s < STATE_COUNT; ++s)
        free(transitions[s].items);
    free(transitions);
    transitions = NULL;
}

seq_t a(idx_t n)
{
    /* Build transition tables once.  */
    init_transitions();

    /* DP arrays.  */
    seq_t *dp = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));

    /* Initial state: all column sums zero, previous row all zero, previous‑prev zero. */
    unsigned int init_state = pack_state(0,0,0, 0,0,0);
    dp[init_state] = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* Zero the next vector. */
        for (unsigned int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (unsigned int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            edge_list e = transitions[s];
            for (unsigned int j = 0; j < e.cnt; ++j)
                dp_next[e.items[j]] += val;
        }
        /* Swap dp and dp_next. */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* Sum all states to obtain the total number of arrays. */
    seq_t answer = 0;
    for (unsigned int i = 0; i < STATE_COUNT; ++i)
        answer += dp[i];

    /* Clean up.  */
    free(dp);
    free(dp_next);
    /* If you want to free the transitions after all calls, uncomment next line. */
    /* free_transitions(); */

    return answer;
}
