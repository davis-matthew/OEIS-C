/*
 * A220619  Number of ways to reciprocally link elements of an nX6 array
 *          either to themselves or to exactly one horizontal, diagonal and
 *          antidiagonal neighbor.
 * https://oeis.org/A220619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the sequence is computed by dynamic programming over rows. For
 *            an n×6 array, each element may link to itself or to exactly one
 *            horizontal, diagonal or antidiagonal neighbour. The algorithm
 *            precomputes all possible link patterns within a single row, then
 *            treats each row as a state transition from...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Width of the array (fixed as 6) */
#define WIDTH 6
#define FULLMASK ((1U << WIDTH) - 1U)

/* Transition information for a single row */
struct trans_t {
    uint8_t next;          /* mask of cells that receive a downward edge from this row */
    seq_t   count;         /* number of ways to achieve that mask from a given source mask */
};

/* Transition tables: trans[m][k] contains the k‑th transition from mask m */
static struct trans_t trans[64][64];
static int trans_cnt[64] = {0};      /* number of transitions for each source mask */
static bool trans_ready = false;    /* flag that tables are ready */

/* Recursive enumeration of all matchings within one row, starting from
   'free_mask' (unset bits are already occupied) and building 'next_mask'
   (bits that obtain a downward edge).  The resulting counts are accumulated
   in 'cnts', indexed by the resulting next_mask. */
static void gen_rec(uint8_t free_mask, uint8_t next_mask, uint64_t *cnts)
{
    if (free_mask == 0) {
        cnts[next_mask] += 1;
        return;
    }
    /* lowest free column */
    int i = __builtin_ctz((unsigned)free_mask);
    /* remove this column from the free mask: it's being processed now */
    free_mask &= (uint8_t)(~(1U << i));

    /* 1. self‑loop – cell i is occupied, nothing added to next_mask */
    gen_rec(free_mask, next_mask, cnts);

    /* 2. horizontal edge to i+1 */
    if (i + 1 < WIDTH && (free_mask & (1U << (i + 1)))) {
        uint8_t fm = free_mask & (uint8_t)(~(1U << (i + 1)));
        gen_rec(fm, next_mask, cnts);
    }

    /* 3. diagonal edge down to i+1 (if not already reserved by another edge) */
    if (i + 1 < WIDTH && !(next_mask & (1U << (i + 1)))) {
        gen_rec(free_mask, (uint8_t)(next_mask | (1U << (i + 1))), cnts);
    }

    /* 4. anti‑diagonal edge down to i‑1 */
    if (i > 0 && !(next_mask & (1U << (i - 1)))) {
        gen_rec(free_mask, (uint8_t)(next_mask | (1U << (i - 1))), cnts);
    }
}

/* Precompute the transition tables once */
static void precompute_transitions(void)
{
    if (trans_ready) return;
    for (int m = 0; m <= FULLMASK; ++m) {
        uint64_t tmp_counts[64] = {0};
        uint8_t free = FULLMASK & (~((uint8_t)m));
        gen_rec(free, 0, tmp_counts);

        int idx = 0;
        for (int t = 0; t <= FULLMASK; ++t) {
            if (tmp_counts[t]) {
                trans[m][idx].next   = (uint8_t)t;
                trans[m][idx].count  = (seq_t)tmp_counts[t];
                ++idx;
            }
        }
        trans_cnt[m] = idx;
    }
    trans_ready = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 for safety */

    precompute_transitions();

    /* DP arrays for the current and next row states (size 2^WIDTH = 64) */
    static seq_t dp[64];
    static seq_t dp2[64];

    /* initialise DP for the first (virtual) row */
    for (int i = 0; i < 64; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Process each real row */
    for (idx_t r = 0; r < n; ++r) {
        for (int i = 0; i < 64; ++i) dp2[i] = 0;
        for (int m = 0; m < 64; ++m) {
            if (dp[m] == 0) continue;
            for (int k = 0; k < trans_cnt[m]; ++k) {
                uint8_t t   = trans[m][k].next;
                seq_t  add  = dp[m] * trans[m][k].count;
                dp2[t] += add;
            }
        }
        for (int i = 0; i < 64; ++i) dp[i] = dp2[i];
    }

    /* After the last row, no pending downward edges may remain */
    return dp[0];
}
