/*
 * A233505  Number of tilings of a 2 X 2 X n box using bricks of shape 3 X 1 X
 *          1 and 2 X 1 X 1.
 * https://oeis.org/A233505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
    A233505
    Formula: Number of tilings of a 2×2×n box using bricks of shape 3×1×1 and 2×1×1.
    The algorithm uses dynamic programming with a 2‑layer lookahead.
    State: (mask0, mask1)
        mask0 – cells already occupied in the current layer,
        mask1 – cells already occupied in the next layer due to bricks started earlier.
    Transition: fill free cells of mask0 with horizontal dominoes, vertical dominoes (2×1×1)
    and trominos (3×1×1).  Vertical dominoes set a bit in mask1; trominos set a bit in
    mask1 and also set a bit in a “future” mask that becomes mask1 of the next step.
    After processing a layer, the new state for the next layer is:
        newMask0 = mask1 | addedMask1
        newMask1 = addedMask2   (there is no longer any occupancy beyond the next layer).
    The answer for n layers is the entry corresponding to (0,0) after the n‑th step.
*/

#include <stddef.h>

 /* adjacency pairs for horizontal domino placement in a 2×2 grid */
static const unsigned int pair_mask[4][2] = {
    {0,1}, {2,3}, {0,2}, {1,3}
};

/* helper recursion to generate all fillings of the current layer */
static void dfs_fill(unsigned int free, unsigned int add1, unsigned int add2,
                     seq_t val, unsigned int m1, seq_t dp_next[16][16]) {
    if (free == 0) {                      /* all cells covered */
        if ((add1 & m1) != 0) return;    /* conflict with existing next layer */
        unsigned int new0 = m1 | add1;    /* next layer becomes current */
        unsigned int new1 = add2;         /* cells for layer after next */
        dp_next[new0][new1] += val;
        return;
    }

    /* find the first free cell */
    unsigned int pos = 0;
    while (pos < 4 && !(free & (1u << pos))) ++pos;

    /* horizontal dominoes involving pos */
    for (int pi = 0; pi < 4; ++pi) {
        unsigned int a = pair_mask[pi][0];
        unsigned int b = pair_mask[pi][1];
        if (pos == a || pos == b) {
            if ((free & (1u << a)) && (free & (1u << b))) {
                unsigned int newfree = free & ~(1u << a) & ~(1u << b);
                dfs_fill(newfree, add1, add2, val, m1, dp_next);
            }
        }
    }

    /* vertical domino (2×1×1) starting at pos */
    if ((m1 & (1u << pos)) == 0) {
        unsigned int newfree = free & ~(1u << pos);
        unsigned int newadd1 = add1 | (1u << pos);
        dfs_fill(newfree, newadd1, add2, val, m1, dp_next);
    }

    /* tromino (3×1×1) starting at pos */
    if ((m1 & (1u << pos)) == 0) {
        unsigned int newfree = free & ~(1u << pos);
        unsigned int newadd1 = add1 | (1u << pos);
        unsigned int newadd2 = add2 | (1u << pos);
        dfs_fill(newfree, newadd1, newadd2, val, m1, dp_next);
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    seq_t dp_cur[16][16] = {0};
    seq_t dp_next[16][16] = {0};

    dp_cur[0][0] = 1;        /* initial empty state */

    for (idx_t layer = 0; layer < n; ++layer) {
        /* clear dp_next */
        for (unsigned int i = 0; i < 16; ++i)
            for (unsigned int j = 0; j < 16; ++j)
                dp_next[i][j] = 0;

        /* process all current states */
        for (unsigned int m0 = 0; m0 < 16; ++m0) {
            for (unsigned int m1 = 0; m1 < 16; ++m1) {
                seq_t val = dp_cur[m0][m1];
                if (val == 0) continue;
                unsigned int free = (~m0) & 0xF;
                dfs_fill(free, 0u, 0u, val, m1, dp_next);
            }
        }

        /* move to next layer */
        for (unsigned int i = 0; i < 16; ++i)
            for (unsigned int j = 0; j < 16; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* final answer: no pending cells in the next layer */
    return dp_cur[0][0];
}
