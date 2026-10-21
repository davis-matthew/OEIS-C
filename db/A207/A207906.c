/*
 * A207906  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A207906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 33, u128 n <= 73,
 *            big any n
 * Helpers:   good_row, good_vertical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A207906
    Number of n×6 binary arrays avoiding the horizontal patterns 000 and 010,
    and the vertical patterns 000 and 011.

    Computation:
    * Enumerate the 25 admissible 6‑bit rows that contain no forbidden
      horizontal 3‑bit substrings.
    * For every ordered pair of rows determine which rows can follow
      while all vertical constraints on the last three rows are satisfied.
      This is stored as a 25×25 bit mask.
    * Use dynamic programming over the rows of the array.
      dp_cur[i·25 + j]  = number of arrays of the current length
                          ending with rows i (second‑last) and j (last).
    * Initialise dp_cur to 1 for every pair (any pair of rows is valid
      when the array has length 2).
    * For each additional row (step 3 … n) update dp_next from dp_cur
      using the pre‑computed masks, then swap the two DP buffers.
    * The answer is the sum of all dp_cur entries after the last step.
    * Base cases: a(1) = 25, a(2) = 625.

    The algorithm runs in O(n·25²) time and uses O(25²) memory
    regardless of the size of seq_t.  The tables are built lazily on
    the first call. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/good_row.h"
#include "../utils/good_vertical.h"

/* seq_t and idx_t are provided by the build environment. */

static int8_t  row_idx[64];          /* map 6‑bit pattern → row index or -1   */
static uint8_t row_masks[25];        /* valid 6‑bit rows                      */
static int16_t row_count;            /* number of valid rows (25)             */

static uint32_t next_mask[625];      /* [row_count][row_count] bits           */

static seq_t *dp_cur_ptr = NULL;     /* pointer to current DP buffer          */
static seq_t *dp_next_ptr = NULL;    /* pointer to next DP buffer            */
static seq_t dp_arr[2][625];         /* two DP buffers                       */

static int built = 0;                /* cache built flag                      */

/* Build static tables on the first call */
static void build_cache(void)
{
    /* initialise row indices to -1 */
    for (int i = 0; i < 64; ++i) row_idx[i] = -1;

    /* find all valid rows */
    row_count = 0;
    for (int p = 0; p < 64; ++p) {
        if (good_row((uint8_t)p)) {
            row_masks[row_count] = (uint8_t)p;
            row_idx[p] = (int8_t)row_count;
            ++row_count;
        }
    }

    /* pre‑compute allowed next rows for each pair */
    for (int i = 0; i < row_count; ++i) {
        for (int j = 0; j < row_count; ++j) {
            uint32_t mask = 0;
            for (int c = 0; c < row_count; ++c) {
                if (good_vertical(row_masks[i], row_masks[j], row_masks[c]))
                    mask |= (1u << c);
            }
            next_mask[i * row_count + j] = mask;
        }
    }

    /* initialise DP buffers */
    dp_cur_ptr = dp_arr[0];
    dp_next_ptr = dp_arr[1];
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Offset 1: undefined at n == 0, return 0 */
    if (n == 0) return (seq_t)0;

    /* Ensure tables are ready */
    if (!built) {
        build_cache();
        built = 1;
    }

    /* Base cases */
    if (n == 1) return (seq_t)row_count;                     /* 25  */
    if (n == 2) return (seq_t)row_count * (seq_t)row_count;  /* 625 */

    /* initialise dp_cur for length 2: any pair of rows is allowed */
    for (int i = 0; i < row_count * row_count; ++i)
        dp_cur_ptr[i] = (seq_t)1;

    /* DP iterations for lengths 3 … n */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear dp_next */
        for (int i = 0; i < row_count * row_count; ++i)
            dp_next_ptr[i] = (seq_t)0;

        /* transition */
        for (int i = 0; i < row_count; ++i) {
            for (int j = 0; j < row_count; ++j) {
                seq_t val = dp_cur_ptr[i * row_count + j];
                if (val == (seq_t)0) continue;
                uint32_t mask = next_mask[i * row_count + j];
                for (int c = 0; c < row_count; ++c) {
                    if (mask & (1u << c))
                        dp_next_ptr[j * row_count + c] += val;
                }
            }
        }

        /* swap buffers */
        seq_t *tmp = dp_cur_ptr;
        dp_cur_ptr = dp_next_ptr;
        dp_next_ptr = tmp;
    }

    /* sum all DP entries to obtain the answer */
    seq_t result = (seq_t)0;
    for (int i = 0; i < row_count * row_count; ++i)
        result += dp_cur_ptr[i];

    return result;
}
