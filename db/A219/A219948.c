/*
 * A219948  Number of tilings of a 7 X n rectangle using right trominoes and 2
 *          X 2 tiles.
 * https://oeis.org/A219948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: DP with a 128‑state transfer matrix for tilings of a 7×n rectangle 
 * using right trominoes (L‑shaped of area 3) and 2×2 squares. 
 * Initial values: a(0)=1, a(1)=0, a(2)=6, a(3)=0, a(4)=92, a(5)=0, ... 
 */

#include <stdint.h>
#include <stddef.h>

#define FULL_MASK 0x7F   /* 7 bits set */

static seq_t trans[128][128];
static int trans_built = 0;

/* Recursive construction of transitions for a given previous mask */
static void build_trans_row(uint8_t curMask, uint8_t nextMask, uint8_t prevMask)
{
    /* When the current column is completely filled */
    if (curMask == FULL_MASK) {
        trans[prevMask][nextMask] += 1;
        return;
    }

    /* Find the first empty cell in the current column */
    uint8_t r = 0;
    while (curMask & (1 << r)) r++;

    /* 1) 2×2 square (requires rows r, r+1 in current column) */
    if (r + 1 < 7 &&
        !(curMask & (1 << (r + 1))) &&
        !(nextMask & ((1 << r) | (1 << (r + 1))))) {
        build_trans_row(curMask | (1 << r) | (1 << (r + 1)),
                        nextMask | (1 << r) | (1 << (r + 1)),
                        prevMask);
    }

    /* 2) Tromino orientation 1: cells (r,0),(r+1,0),(r+1,1) */
    if (r + 1 < 7 &&
        !(curMask & (1 << (r + 1))) &&
        !(nextMask & (1 << (r + 1)))) {
        build_trans_row(curMask | (1 << r) | (1 << (r + 1)),
                        nextMask | (1 << (r + 1)),
                        prevMask);
    }

    /* 3) Tromino orientation 2: cells (r,0),(r+1,0),(r,1) */
    if (r + 1 < 7 &&
        !(curMask & (1 << (r + 1))) &&
        !(nextMask & (1 << r))) {
        build_trans_row(curMask | (1 << r) | (1 << (r + 1)),
                        nextMask | (1 << r),
                        prevMask);
    }

    /* 4) Tromino orientation 3: cells (r,0),(r,1),(r+1,1) */
    if (r + 1 < 7 &&
        !(nextMask & ((1 << r) | (1 << (r + 1))))) {
        build_trans_row(curMask | (1 << r),
                        nextMask | (1 << r) | (1 << (r + 1)),
                        prevMask);
    }

    /* 5) Tromino orientation 4: cells (r,0),(r,1),(r-1,1) */
    if (r > 0 &&
        !(nextMask & ((1 << r) | (1 << (r - 1))))) {
        build_trans_row(curMask | (1 << r),
                        nextMask | (1 << r) | (1 << (r - 1)),
                        prevMask);
    }
}

/* Build all transitions once */
static void build_transitions(void)
{
    for (uint8_t p = 0; p < 128; ++p) {
        for (uint8_t q = 0; q < 128; ++q)
            trans[p][q] = 0;
        build_trans_row(p, 0, p);
    }
    trans_built = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!trans_built)
        build_transitions();

    seq_t dp_prev[128] = {0};
    seq_t dp_next[128] = {0};

    dp_prev[0] = 1;   /* empty start state */

    for (idx_t col = 0; col < n; ++col) {
        /* zero next row */
        for (uint8_t i = 0; i < 128; ++i)
            dp_next[i] = 0;

        for (uint8_t prev = 0; prev < 128; ++prev) {
            if (dp_prev[prev] == 0) continue;
            for (uint8_t next = 0; next < 128; ++next) {
                seq_t ways = trans[prev][next];
                if (!ways) continue;
                dp_next[next] += dp_prev[prev] * ways;
            }
        }

        /* swap dp_prev and dp_next */
        for (uint8_t i = 0; i < 128; ++i)
            dp_prev[i] = dp_next[i];
    }

    return dp_prev[0];   /* final state with no protrusions */
}
