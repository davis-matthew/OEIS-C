/*
 * A028460  Number of perfect matchings in graph P_{3} X C_{3} X P_{2n}.
 * https://oeis.org/A028460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   This implementation computes the number of perfect matchings in
 *            the graph P3 × C3 × P2n using dynamic programming over the 2n
 *            layers of the 3×3 grid. It is not a closed form formula but is
 *            efficient O(n) with a small constant
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*------------------------------------------------------------*/
/* Constants and global tables                               */
/*------------------------------------------------------------*/
#define STATE 512          /* 3×3 grid, 9 vertices -> 2^9 states */
#define MAX_TRANS 256      /* an upper bound on transitions per state */

/* Neighbor masks for the 3×3 graph with cyclic horizontal edges. */
static uint16_t neighbour_mask[9];

/* Transition tables: for each input mask we store all possible output
   masks and the number of ways to realize that transition. */
static uint16_t  trans_masks[STATE][MAX_TRANS];
static seq_t     trans_counts[STATE][MAX_TRANS];
static uint16_t  trans_len[STATE];

/* Dynamic programming state vectors. */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static idx_t  dp_cur_len = 0;   /* 0 means uninitialised */

/* Cached answers. */
static seq_t *answers = NULL;
static idx_t answered_to = 0;    /* largest n already computed */

static idx_t current_layer = 0;   /* layers processed so far */

/*------------------------------------------------------------*/
/* Helper: initialise neighbour masks                        */
/*------------------------------------------------------------*/
static void init_neighbours(void)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            int idx = i * 3 + j;
            uint16_t m = 0;
            /* vertical neighbours (P3) */
            if (i > 0)       m |= 1u << ((i - 1) * 3 + j);
            if (i < 2)       m |= 1u << ((i + 1) * 3 + j);
            /* horizontal neighbours (C3) - cyclic */
            int j1 = (j + 1) % 3;
            int j2 = (j + 2) % 3;  /* j-1 modulo 3 */
            m |= 1u << (i * 3 + j1);
            m |= 1u << (i * 3 + j2);
            neighbour_mask[idx] = m;
        }
}

/*------------------------------------------------------------*/
/* Recursive enumeration of matchings for a single layer.     */
/*------------------------------------------------------------*/
static void dfs_match(uint16_t used, uint16_t mask_out,
                      uint16_t mask_in, seq_t *counts)
{
    if (used == 0x1FFu) {                 /* all 9 vertices used */
        counts[mask_out] += 1;
        return;
    }

    /* first free vertex */
    int v;
    for (v = 0; v < 9; ++v)
        if (!(used & (1u << v)))
            break;

    /* Match v downwards to the next layer. */
    dfs_match(used | (1u << v), mask_out | (1u << v),
              mask_in, counts);

    /* Match v inside the current layer with an upward neighbour. */
    uint16_t neigh = neighbour_mask[v] & ~used;
    while (neigh) {
        int w = __builtin_ctz(neigh);
        neigh &= neigh - 1;
        if (w <= v)   /* avoid double counting each undirected edge */
            continue;
        dfs_match(used | (1u << v) | (1u << w),
                  mask_out, mask_in, counts);
    }
}

/*------------------------------------------------------------*/
/* Build all transition tables once.                          */
/*------------------------------------------------------------*/
static void init_transitions(void)
{
    static bool built = false;
    if (built) return;
    init_neighbours();

    for (int mask_in = 0; mask_in < STATE; ++mask_in) {
        seq_t counts[STATE];
        memset(counts, 0, sizeof counts);
        dfs_match((uint16_t)mask_in, 0, (uint16_t)mask_in, counts);

        uint16_t len = 0;
        for (int mask_out = 0; mask_out < STATE; ++mask_out)
            if (counts[mask_out]) {
                trans_masks[mask_in][len] = (uint16_t)mask_out;
                trans_counts[mask_in][len] = counts[mask_out];
                ++len;
            }
        trans_len[mask_in] = len;
    }
    built = true;
}

/*------------------------------------------------------------*/
/* Allocate DP vectors if not yet done.                       */
/*------------------------------------------------------------*/
static void ensure_dp_vectors(void)
{
    if (dp_cur_len) return;
    dp_cur = (seq_t *)calloc(STATE, sizeof(seq_t));
    dp_next = (seq_t *)calloc(STATE, sizeof(seq_t));
    dp_cur_len = STATE;
    /* initial state before any layer: no pending edges. */
    dp_cur[0] = 1;
}

/*------------------------------------------------------------*/
/* Ensure answers array is large enough.                      */
/*------------------------------------------------------------*/
static void ensure_answers(size_t sz)
{
    if (answers && (answered_to + 1) >= sz) return;
    seq_t *new_arr = (seq_t *)calloc(sz, sizeof(seq_t));
    if (answers) {
        memcpy(new_arr, answers, sizeof(seq_t) * (answered_to + 1));
        free(answers);
    }
    answers = new_arr;
}

/*------------------------------------------------------------*/
/* Compute sequence values up to index n.                     */
/*------------------------------------------------------------*/
static void compute_until(idx_t n)
{
    init_transitions();
    ensure_dp_vectors();

    /* Set up answers array. */
    ensure_answers(n + 1);
    if (answered_to == 0) answers[0] = 1;   /* a(0) = 1 */

    /* Process layers from current_layer to target_layer. */
    idx_t target_layer = 2 * n;
    while (current_layer < target_layer) {
        /* reset next layer DP vector */
        memset(dp_next, 0, sizeof(seq_t) * STATE);

        /* transition from current state to next */
        for (uint16_t mask_in = 0; mask_in < STATE; ++mask_in) {
            if (dp_cur[mask_in] == 0) continue;
            seq_t val = dp_cur[mask_in];
            for (uint16_t t = 0; t < trans_len[mask_in]; ++t) {
                uint16_t mask_out = trans_masks[mask_in][t];
                seq_t    coeff = trans_counts[mask_in][t];
                dp_next[mask_out] += val * coeff;
            }
        }

        /* swap dp vectors */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        ++current_layer;

        /* After completing an even number of layers (one full P2 block),
           we have the value for a(n). */
        if ((current_layer & 1) == 0) {
            idx_t idx = current_layer >> 1;      /* current n */
            if (idx > (idx_t)answered_to) {
                answers[idx] = dp_cur[0];
                answered_to = idx;
            }
        }
    }
}

/*------------------------------------------------------------*/
/* Public function                                                */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    compute_until(n);
    return answers[n];
}
