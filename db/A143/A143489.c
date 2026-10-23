/*
 * A143489  "Fourth down, Extream [sic] between the two farthest Bells from
 *          it" in bell-ringing is a sequence of permutations p_1=(1,2,3,4),
 *          p_2=(1,2,4,3), .. which runs through all permutations of {1,2,3,4}
 *          with period 24; sequence gives position of bell 2 in n-th
 *          permutation.
 * https://oeis.org/A143489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-12) + a(n-13) for n > 13, with period 24.
 *            Sequence is obtained by generating the bell‑ringing permutation
 *            order described in the OEIS entry
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the 24‑term cycle once and
   returns the appropriate value for any n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cycle = NULL;   /* 24 precomputed cycle values */

/* 3‑digit block permutations used by the algorithm.  They are
   produced by the same process applied to {1,2,3}. */
static const seq_t P3[6][3] = {
    {1, 2, 3},      /* 0 */
    {1, 3, 2},      /* 1 */
    {3, 1, 2},      /* 2 */
    {3, 2, 1},      /* 3 */
    {2, 3, 1},      /* 4 */
    {2, 1, 3}       /* 5 */
};

static void init_cycle(void) {
    if (cycle) return; /* already initialised */

    cycle = (seq_t *)malloc(24 * sizeof(seq_t));
    /* permutation array for the 4‑bell state */
    seq_t perm[4] = {1, 2, 3, 4};
    int dir = -1;          /* -1 : move 4 left, +1 : move 4 right */
    int block = 0;         /* index into P3, starting with {1,2,3} */

    for (idx_t i = 0; i < 24; ++i) {
        /* record position of bell 2 (1‑based) */
        for (int j = 0; j < 4; ++j) {
            if (perm[j] == 2) {
                cycle[i] = (seq_t)(j + 1);
                break;
            }
        }

        if (i == 23) break;  /* last term done */

        /* find current position of bell 4 */
        int idx4 = -1;
        for (int j = 0; j < 4; ++j) {
            if (perm[j] == 4) {
                idx4 = j;
                break;
            }
        }

        if (dir == -1) {                 /* moving left */
            if (idx4 > 0) {               /* simple swap left */
                seq_t tmp = perm[idx4];
                perm[idx4] = perm[idx4 - 1];
                perm[idx4 - 1] = tmp;
            } else {                      /* 4 is at position 0 */
                /* switch to next block and change direction */
                block = (block + 1) % 6;
                perm[1] = P3[block][0];
                perm[2] = P3[block][1];
                perm[3] = P3[block][2];
                dir = +1;
            }
        } else {                         /* dir == +1, moving right */
            if (idx4 < 3) {               /* simple swap right */
                seq_t tmp = perm[idx4];
                perm[idx4] = perm[idx4 + 1];
                perm[idx4 + 1] = tmp;
            } else {                      /* 4 is at position 3 */
                /* switch to next block and change direction */
                block = (block + 1) % 6;
                perm[0] = P3[block][0];
                perm[1] = P3[block][1];
                perm[2] = P3[block][2];
                /* perm[3] is already 4 */
                dir = -1;
            }
        }
    }
}

seq_t a(idx_t n) {
    init_cycle();                    /* build cycle on first call */
    return cycle[(n - 1) % 24];      /* period 24, offset 1 */
}
