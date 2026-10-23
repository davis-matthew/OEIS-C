/*
 * A180766  Half the number of nX6 binary arrays with each element equal to at
 *          least one neighbor.
 * https://oeis.org/A180766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (# of n×6 binary arrays in which every cell
 *               has at least one neighbor with the same value) / 2.
 * This is implemented by dynamic programming over the rows.
 */

#include <stdint.h>
#include <stdbool.h>

#define WIDTH          6
#define NUM_PAT        (1U << WIDTH)      /* 64 */
#define NUM_STATES     (NUM_PAT * NUM_PAT)/* 4096 */
#define NEED_MASK      (NUM_PAT - 1)

static uint8_t unsat_h_mask[NUM_PAT];
static unsigned char trans_len[NUM_STATES];
static uint16_t trans_dest[NUM_STATES][NUM_PAT];
static bool init_done = false;

static void init_precompute(void)
{
    /* compute unsatisfied horizontal mask for each row pattern */
    for (int r = 0; r < NUM_PAT; ++r) {
        uint8_t mask = 0;
        for (int c = 0; c < WIDTH; ++c) {
            int bit = (r >> c) & 1;
            bool left_eq  = false;
            bool right_eq = false;
            if (c > 0) {
                int lbit = (r >> (c - 1)) & 1;
                left_eq = (lbit == bit);
            }
            if (c < WIDTH - 1) {
                int rbit = (r >> (c + 1)) & 1;
                right_eq = (rbit == bit);
            }
            if (!left_eq && !right_eq)
                mask |= (1U << c);
        }
        unsat_h_mask[r] = mask;
    }

    /* precompute transition lists for all states (prev_row, prev_need) */
    for (int pp = 0; pp < NUM_PAT; ++pp) {
        for (int pn = 0; pn < NUM_PAT; ++pn) {
            int src = (pp << WIDTH) | pn;
            unsigned char cnt = 0;
            for (int c = 0; c < NUM_PAT; ++c) {
                uint8_t diff = pp ^ c;
                if ((pn & diff) != 0)   /* columns that must match but don't */
                    continue;
                uint8_t new_need = unsat_h_mask[c] & diff;
                int dest = (c << WIDTH) | new_need;
                trans_dest[src][cnt++] = (uint16_t)dest;
            }
            trans_len[src] = cnt;
        }
    }

    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done)
        init_precompute();

    static seq_t dp[NUM_STATES];
    static seq_t ndp[NUM_STATES];

    /* clear dp */
    for (int i = 0; i < NUM_STATES; ++i)
        dp[i] = 0;

    /* initial row: any pattern, need = unsatisfied horizontal positions */
    for (int r = 0; r < NUM_PAT; ++r) {
        int state = (r << WIDTH) | unsat_h_mask[r];
        dp[state] = 1;
    }

    /* iterate over remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        /* clear ndp */
        for (int i = 0; i < NUM_STATES; ++i)
            ndp[i] = 0;

        for (int src = 0; src < NUM_STATES; ++src) {
            seq_t val = dp[src];
            if (val == 0)
                continue;
            unsigned char len = trans_len[src];
            uint16_t *dest_arr = trans_dest[src];
            for (unsigned char t = 0; t < len; ++t) {
                ndp[dest_arr[t]] += val;
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < NUM_STATES; ++i)
            dp[i] = ndp[i];
    }

    /* sum all configurations where no unsatisfied column remains */
    seq_t total = 0;
    for (int state = 0; state < NUM_STATES; ++state) {
        if ((state & NEED_MASK) == 0)
            total += dp[state];
    }

    return total / 2;   /* the sequence stores half the total count */
}
