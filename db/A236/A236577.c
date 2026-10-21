/*
 * A236577  The number of tilings of a 6 X n floor with 1 X 3 trominoes.
 * https://oeis.org/A236577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A236577
    a(n) = number of tilings of a 6×n board with 1×3 trominoes
    Computation via transfer‑matrix DP over 12‑bit states.
    The algorithm precomputes transition lists for all 4096 states
    and performs DP in O(n·states) time. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- transition table structures ---------------------------------- */
typedef struct {
    unsigned short dest;   /* destination state (0 .. 4095)      */
    unsigned char count;   /* number of ways to reach dest      */
} trans_entry_t;

/* number of test cases up to 4096 states (2^12) */
#define NUM_STATES 4096

/* transition counts and arrays */
static size_t trans_len[NUM_STATES];
static trans_entry_t *trans[NUM_STATES];

/* DP storage */
static seq_t dp_arr[NUM_STATES];
static seq_t next_arr[NUM_STATES];

/* ------------------------------------------------------------------- */
/* ... helper function to generate all tilings of a single column ...  */
static void dfs_fill(uint16_t cur_mask, uint16_t next_mask, uint16_t two_mask,
                     int *dest_counts)
{
    /* find first empty row in the current column */
    int r;
    for (r = 0; r < 6; ++r)
        if (!(cur_mask & (1U << r)))
            break;

    /* all rows filled -> record destination state */
    if (r == 6) {
        unsigned short dest_state = next_mask | (two_mask << 6);
        dest_counts[dest_state]++;
        return;
    }

    /* place a vertical tromino (3 cells in the same column) */
    if (r <= 3 &&
        !(cur_mask & (1U << (r + 1))) &&
        !(cur_mask & (1U << (r + 2)))) {
        uint16_t new_cur = cur_mask | (1U << r) | (1U << (r + 1)) | (1U << (r + 2));
        dfs_fill(new_cur, next_mask, two_mask, dest_counts);
    }

    /* place a horizontal tromino (spans current, next, and next‑next column) */
    if (!(next_mask & (1U << r)) && !(two_mask & (1U << r))) {
        uint16_t new_cur    = cur_mask | (1U << r);
        uint16_t new_next   = next_mask | (1U << r);
        uint16_t new_two    = two_mask   | (1U << r);
        dfs_fill(new_cur, new_next, new_two, dest_counts);
    }
}

/* ------------------------------------------------------------------- */
static void init_transitions(void)
{
    static bool initialized = false;
    if (initialized)
        return;

    /* temporary array for counting destinations from a given source */
    int dest_counts[NUM_STATES];

    for (int src = 0; src < NUM_STATES; ++src) {
        uint16_t cur_mask  = src & 0x3F;       /* lower 6 bits  */
        uint16_t next_mask = (src >> 6) & 0x3F;/* upper 6 bits */

        /* clear destination counts */
        for (int i = 0; i < NUM_STATES; ++i)
            dest_counts[i] = 0;

        dfs_fill(cur_mask, next_mask, 0, dest_counts);

        /* store only non‑zero destinations */
        size_t cnt = 0;
        for (int d = 0; d < NUM_STATES; ++d)
            if (dest_counts[d])
                ++cnt;

        trans_len[src] = cnt;
        if (cnt == 0) {
            trans[src] = NULL;
            continue;
        }

        trans[src] = (trans_entry_t *)malloc(cnt * sizeof(trans_entry_t));
        size_t idx = 0;
        for (int d = 0; d < NUM_STATES; ++d) {
            if (dest_counts[d]) {
                trans[src][idx].dest  = (unsigned short)d;
                trans[src][idx].count = (unsigned char)dest_counts[d];
                ++idx;
            }
        }
    }

    initialized = true;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* initialize transition tables once */
    init_transitions();

    /* set up DP arrays */
    seq_t *curdp  = dp_arr;
    seq_t *nextdp = next_arr;

    /* clear current DP and set start state */
    for (int i = 0; i < NUM_STATES; ++i)
        curdp[i] = (seq_t)0;
    curdp[0] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        /* clear next DP */
        for (int i = 0; i < NUM_STATES; ++i)
            nextdp[i] = (seq_t)0;

        /* perform transitions */
        for (int src = 0; src < NUM_STATES; ++src) {
            seq_t val = curdp[src];
            if (val == (seq_t)0)
                continue;
            size_t tlen = trans_len[src];
            for (size_t j = 0; j < tlen; ++j) {
                trans_entry_t e = trans[src][j];
                nextdp[e.dest] += val * (seq_t)e.count;
            }
        }

        /* swap dp pointers for next iteration */
        seq_t *tmp = curdp;
        curdp = nextdp;
        nextdp = tmp;
    }

    return curdp[0];
}
