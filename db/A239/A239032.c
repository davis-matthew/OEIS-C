/*
 * A239032  Number of 5 X n 0..2 arrays with no element equal to the sum of
 *          elements to its left or one plus the sum of the elements above it,
 *          modulo 3.
 * https://oeis.org/A239032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 93, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239032:
   Number of 5 × n 0..2 arrays with no element equal to
   (sum of elements to its left) mod 3
   or equal to 1 + (sum of elements above it) mod 3.
   This implementation uses a dynamic‑programming state machine
   with 3⁵ = 243 states.  For each column the transition from an
   initial state of left‑sums (mod 3) to the new state is
   pre‑computed.  The recurrence a(n)=7a(n‑1)−21a(n‑2)+…+a(n‑7)
   holds for n>8 but the code does not hard‑code any terms. */

#include <stdlib.h>
#include <stdbool.h>

/* constants */
#define STATE_COUNT 243
#define ROWS 5
#define MAX_TRANS 32

/* transition structure */
typedef struct {
    unsigned char to[MAX_TRANS];   /* target state index */
    unsigned char weight[MAX_TRANS]; /* number of ways for this transition */
    unsigned char count;           /* number of distinct transitions */
} Trans;

/* global transition table */
static Trans trans[STATE_COUNT];

/* flag for one–time construction of transitions */
static bool trans_inited = false;

/* helper: encode a 5‑digit base‑3 number to an int 0..242 */
static inline int encode(const int a[ROWS]) {
    int s = 0;
    for (int i = 0; i < ROWS; ++i)
        s = s * 3 + a[i];
    return s;
}

/* depth‑first search to build transitions for one initial state */
static void dfs_build(int row, int col_sum,
                      const int orig[ROWS], int cur[ROWS],
                      Trans *t)
{
    if (row == ROWS) {
        int next = encode(cur);
        /* search if this next state already exists */
        for (int k = 0; k < t->count; ++k)
            if (t->to[k] == next) {
                t->weight[k] += 1;
                return;
            }
        t->to[t->count] = (unsigned char)next;
        t->weight[t->count] = 1;
        t->count++;
        return;
    }
    int forb1 = orig[row];
    int forb2 = (1 + col_sum) % 3;
    for (int v = 0; v < 3; ++v) {
        if (v == forb1 || v == forb2) continue;          /* forbidden */
        cur[row] = (orig[row] + v) % 3;
        dfs_build(row + 1, (col_sum + v) % 3, orig, cur, t);
    }
}

/* build all transitions once */
static void init_transitions(void) {
    for (int s = 0; s < STATE_COUNT; ++s) {
        /* decode initial state */
        int orig[ROWS];
        int tmp = s;
        for (int i = ROWS - 1; i >= 0; --i) {
            orig[i] = tmp % 3;
            tmp /= 3;
        }
        trans[s].count = 0;
        int cur[ROWS];
        dfs_build(0, 0, orig, cur, &trans[s]);
    }
    trans_inited = true;
}

/* main function implementing the OEIS sequence */
seq_t a(idx_t n)
{
    /* safety for n==0 */
    if (n == 0) return 0;

    /* initialize transitions once */
    if (!trans_inited) init_transitions();

    /* DP arrays: two rings of size STATE_COUNT */
    static seq_t dp_cur[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];
    static bool dp_inited = false;

    if (!dp_inited) {
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;
        dp_cur[0] = 1;           /* all left sums start at 0 */
        dp_inited = true;
    } else {
        /* reset dp_cur for a new computation */
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;
        dp_cur[0] = 1;
    }
    /* perform n column transitions */
    for (idx_t step = 0; step < n; ++step) {
        /* clear next array */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;
        /* propagate */
        for (int s = 0; s < STATE_COUNT; ++s) {
            if (dp_cur[s] == 0) continue;
            Trans *t = &trans[s];
            for (int k = 0; k < t->count; ++k) {
                seq_t add = dp_cur[s] * (seq_t)(t->weight[k]);
                dp_next[t->to[k]] += add;
            }
        }
        /* swap rings */
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all final states */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += dp_cur[i];
    return total;
}
