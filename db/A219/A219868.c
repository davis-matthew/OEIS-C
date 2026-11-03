/*
 * A219868  Number of tilings of a 5 X n rectangle using dominoes and straight
 *          (3 X 1) trominoes.
 * https://oeis.org/A219868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 500 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: dynamic programming over a 15‑bit state encoding
 * occupancy of the current column and the next two columns.
 * No closed‑form formula is known; the DP iterates over columns,
 * generating all ways to fill the current column with dominoes
 * (2×1) and straight trominoes (3×1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Recursive generator that explores all ways to fill the current column.
 * m0, m1, m2 are the occupancy masks of column 0 (current), 1, and 2.
 * tmp collects the raw next states (before deduplication).
 * len accumulates the number of collected states. */
static void rec_expand(unsigned m0, unsigned m1, unsigned m2,
                       unsigned *tmp, size_t *len)
{
    int r;
    for (r = 0; r < 5; ++r)
        if (!(m0 & (1u << r))) break;

    if (r == 5) { /* current column fully filled */
        unsigned to = (m1 << 10) | (m2 << 5); /* column 3 mask is always 0 */
        tmp[(*len)++] = to;
        return;
    }

    /* place a vertical domino (size 2x1) */
    if (r + 1 < 5 && !(m0 & (1u << (r + 1)))) {
        rec_expand(m0 | (1u << r) | (1u << (r + 1)),
                   m1, m2, tmp, len);
    }

    /* place a vertical tromino (size 3x1) */
    if (r + 2 < 5 &&
        !(m0 & (1u << (r + 1))) &&
        !(m0 & (1u << (r + 2)))) {
        rec_expand(m0 | (1u << r) | (1u << (r + 1)) | (1u << (r + 2)),
                   m1, m2, tmp, len);
    }

    /* place a horizontal domino (size 1x2) spanning columns 0 and 1 */
    if (!(m1 & (1u << r))) {
        rec_expand(m0 | (1u << r),
                   m1 | (1u << r), m2, tmp, len);
    }

    /* place a horizontal tromino (size 1x3) spanning columns 0,1,2 */
    if (!(m1 & (1u << r)) && !(m2 & (1u << r))) {
        rec_expand(m0 | (1u << r),
                   m1 | (1u << r), m2 | (1u << r), tmp, len);
    }
}

/* transition entry: destination state and multiplicity of the transition */
typedef struct {
    unsigned to;   /* next state (15‑bit mask) */
    seq_t ways;    /* number of ways to reach it from the current state */
} Trans;

/* Number of possible 15‑bit states: 5 bits per column, 3 columns */
static const unsigned NUM_STATES = 1U << 15;

/* Tables of precomputed transitions; built lazily */
static Trans **trans_table  = NULL;   /* array of pointers to Trans lists */
static size_t *trans_counts = NULL;  /* length of each list */

/* For marking reachable states */
static unsigned  *reachable_states = NULL;  /* array of reachable state indices */
static size_t    reachable_count  = 0;
static unsigned char *state_visited = NULL;

/* Simple comparison for qsort */
static int cmp_uint(const void *p, const void *q)
{
    unsigned a = *(const unsigned *)p;
    unsigned b = *(const unsigned *)q;
    return (a > b) - (a < b);
}

/* Build transition list for state s (= [m0 m1 m2] as a 15‑bit number) */
static void generate_transitions(unsigned s)
{
    unsigned m0 = s >> 10;
    unsigned m1 = (s >> 5) & 0x1F;
    unsigned m2 = s & 0x1F;

    /* 8192 is ample for all intermediate states */
    unsigned tmp[8192];
    size_t tmp_len = 0;

    rec_expand(m0, m1, m2, tmp, &tmp_len);

    qsort(tmp, tmp_len, sizeof(unsigned), cmp_uint);

    /* collapse duplicates, counting multiplicities */
    size_t uniq = 0;
    Trans *list = mem_new(Trans, tmp_len);
    for (size_t i = 0; i < tmp_len; ) {
        unsigned val = tmp[i];
        size_t j = i + 1;
        while (j < tmp_len && tmp[j] == val) ++j;
        list[uniq].to = val;
        list[uniq].ways = (seq_t)(j - i);
        ++uniq;
        i = j;
    }

    trans_table[s] = list;
    trans_counts[s] = uniq;
}

/* Build the set of states reachable from the empty state using the
 * transition system.  This reduces the number of states that must
 * be processed in each DP step. */
static void build_reachable_states(void)
{
    /* one‑time allocation of auxiliary storage */
    state_visited = (unsigned char *)calloc(NUM_STATES, sizeof(unsigned char));
    reachable_states = (unsigned *)malloc(NUM_STATES * sizeof(unsigned));

    unsigned *queue = (unsigned *)malloc(NUM_STATES * sizeof(unsigned));
    size_t head = 0, tail = 0;

    queue[tail++] = 0;    /* start from the empty mask */

    while (head < tail) {
        unsigned s = queue[head++];
        if (state_visited[s]) continue;
        state_visited[s] = 1;
        reachable_states[reachable_count++] = s;

        /* ensure transitions for this state are available */
        if (trans_table[s] == NULL)
            generate_transitions(s);

        Trans *list = trans_table[s];
        size_t len  = trans_counts[s];
        for (size_t t = 0; t < len; ++t) {
            unsigned next = list[t].to;
            if (!state_visited[next]) {
                queue[tail++] = next;
            }
        }
    }

    free(queue);
}

/* Main entry function */
seq_t a(idx_t n)
{
    /* DP arrays: one for current column, one for next */
    static seq_t *dp_cur  = NULL;
    static seq_t *dp_next = NULL;

    if (dp_cur == NULL) {
        dp_cur  = mem_new(seq_t, NUM_STATES);
        dp_next = mem_new(seq_t, NUM_STATES);
    }

    /* transition tables: created on first use */
    if (trans_table == NULL) {
        trans_table  = (Trans **)calloc(NUM_STATES, sizeof(Trans *));
        trans_counts = (size_t *)calloc(NUM_STATES, sizeof(size_t));
    }

    /* Build reachable states once (cheap) */
    if (reachable_states == NULL)
        build_reachable_states();

    /* initialise DP for column 0 */
    for (size_t i = 0; i < NUM_STATES; ++i) dp_cur[i] = 0;
    dp_cur[0] = 1;

    /* iterate over columns */
    for (idx_t col = 0; col < n; ++col) {
        /* clear next DP array */
        for (size_t i = 0; i < NUM_STATES; ++i) dp_next[i] = 0;

        /* process all reachable states only */
        for (size_t i = 0; i < reachable_count; ++i) {
            unsigned s = reachable_states[i];
            seq_t cur = dp_cur[s];
            if (cur == 0) continue;

            if (trans_table[s] == NULL) generate_transitions(s);

            Trans *list = trans_table[s];
            size_t len  = trans_counts[s];
            for (size_t t = 0; t < len; ++t) {
                dp_next[list[t].to] += cur * list[t].ways;
            }
        }

        /* swap current and next DP arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* final state must be empty mask (0) */
    return dp_cur[0];
}
