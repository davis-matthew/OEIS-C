/*
 * A226543  Number of tilings of a 9 X n rectangle using 1 X 1 squares and
 *          L-tiles.
 * https://oeis.org/A226543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence via width‑9 column DP a(0) = 1 For each column a( n )
 *            = Σ over masks of tiles that fill that column Transition: from
 *            mask of cells occupied by the right side of previous L‑tiles to
 *            new mask formed by the L‑tiles starting in this column
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are provided by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* 9 rows => 512 masks */
#define ROWS 9
#define MASKS (1LU << ROWS)

/* Structure to store a transition (output mask and count) */
typedef struct {
    uint16_t out;
    seq_t    cnt;
} Trans;

/* Global static data for memoization */
static Trans transitions[MASKS][MASKS];
static uint8_t  trans_len[MASKS] = {0};
static int     transitions_done = 0;

static seq_t dp_cur[MASKS];
static seq_t dp_next[MASKS];
static seq_t results[10001];
static idx_t  computed_up_to = 0;         /* number of columns already processed */

/* ---------------------------------------------------------------------- */
/* Utility: zero an array of seq_t */
static void zero_seq_array(seq_t *arr, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        arr[i] = (seq_t)0;
}

/* ---------------------------------------------------------------------- */
/* DFS to generate all tilings for a given starting mask */
/* local_cnt[outMask] accumulates counts for this starting mask */
static void dfs_tiling(uint16_t usedMask, uint16_t outMask,
                       seq_t *local_cnt)
{
    /* Find first free row */
    int r;
    for (r = 0; r < ROWS; ++r)
        if (!(usedMask & (1U << r)))
            break;

    if (r == ROWS) {
        /* All rows filled */
        local_cnt[outMask] = local_cnt[outMask] + (seq_t)1;
        return;
    }

    /* Place a 1x1 square */
    dfs_tiling(usedMask | (1U << r), outMask, local_cnt);

    /* Place an L‑tile if possible */
    if (r + 1 < ROWS && !(usedMask & (1U << (r + 1))) &&
        !(usedMask & (1U << r))) {
        uint16_t new_used = usedMask | (1U << r) | (1U << (r + 1));
        uint16_t new_out  = outMask | (1U << (r + 1));
        dfs_tiling(new_used, new_out, local_cnt);
    }
}

/* ---------------------------------------------------------------------- */
/* Compute all transitions once */
static void compute_transitions(void)
{
    if (transitions_done)
        return;

    seq_t local_cnt[MASKS];

    for (int inMask = 0; inMask < (int)MASKS; ++inMask) {
        /* Zero local counter */
        for (int i = 0; i < (int)MASKS; ++i)
            local_cnt[i] = (seq_t)0;

        /* Generate all tilings for this starting mask */
        dfs_tiling((uint16_t)inMask, 0, local_cnt);

        /* Store non‑zero transitions */
        uint8_t count = 0;
        for (int outMask = 0; outMask < (int)MASKS; ++outMask) {
            if (local_cnt[outMask] != (seq_t)0) {
                transitions[inMask][count].out = (uint16_t)outMask;
                transitions[inMask][count].cnt = local_cnt[outMask];
                ++count;
            }
        }
        trans_len[inMask] = count;
    }
    transitions_done = 1;
}

/* ---------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n >= 10001)
        abort(); /* out of bounds for preallocated array */

    /* Initialisation on first call */
    if (!transitions_done) {
        /* Prepare transitions */
        compute_transitions();

        /* Initialise DP for 0 columns */
        for (int i = 0; i < (int)MASKS; ++i)
            dp_cur[i] = (seq_t)0;
        dp_cur[0] = (seq_t)1;
        /* First term */
        results[0] = (seq_t)1;
        computed_up_to = 0;
    }

    /* Compute up to n if not already done */
    for (idx_t col = computed_up_to; col < n; ++col) {
        zero_seq_array(dp_next, MASKS);   /* empty next column */

        for (int inMask = 0; inMask < (int)MASKS; ++inMask) {
            if (dp_cur[inMask] == (seq_t)0)
                continue;

            for (uint8_t k = 0; k < trans_len[inMask]; ++k) {
                Trans t = transitions[inMask][k];
                seq_t prod = dp_cur[inMask] * t.cnt;
                dp_next[t.out] = dp_next[t.out] + prod;
            }
        }

        /* Move to next column */
        for (int i = 0; i < (int)MASKS; ++i)
            dp_cur[i] = dp_next[i];

        /* Store result for this column count */
        results[col + 1] = dp_cur[0];
    }

    computed_up_to = n;
    return results[n];
}
