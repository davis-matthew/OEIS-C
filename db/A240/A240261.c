/*
 * A240261  Number of 2Xn 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of the elements
 *          above it or one plus the sum of the elements diagonally to its
 *          northwest or one plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A240261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 2×n arrays over {0,1,2,3} avoiding v = 1 + (sum of
 *            elements left) or v = 1 + (sum of elements above) or v = 1 +
 *            (sum of elements northwest) or v = 1 + (sum of elements
 *            northeast) (all sums mod 4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The DP keeps state as (sumTop, sumBottom, prevTop, prevBottom).
   sumX are the sums mod 4 of the corresponding row up to the
   previous column.  At each step we enumerate all possible
   current column values and update the sums.  A bottom cell
   may forbid the top cell of the next column via the
   south‑east (NE) condition, so this constraint is checked
   when moving to the next column.  After the last column,
   the NE constraint for the last bottom cell reduces to
   forbidding value 1.  The routine is O(n) in the number of
   columns and uses only integer arithmetic. */

#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1, behaviour for n=0 is irrelevant */

    /* state index encoding:
         bits 7‑6 : sumTop   (0…3)
         bits 5‑4 : sumBottom(0…3)
         bits 3‑2 : lastTop
         bits 1‑0 : lastBottom
    */
    static seq_t prev[256];
    static seq_t next[256];

    memset(prev, 0, sizeof(prev));

    /* ---- initial column (col = 0) ---- */
    for (int a0 = 0; a0 < 4; ++a0) {
        /* top cell: forbidden if a0 == 1 (sum_above = 0) or a0 == 1 + sum_left (sum_left = 0) */
        if (a0 == 1)
            continue;                   /* forbids a0 == 1 */

        for (int a1 = 0; a1 < 4; ++a1) {
            /* bottom cell: forbidden if a1 == 1 + sum_left_bottom
                           or a1 == 1 + sum_above_bottom
                           (the latter is 1 + a0) */
            if (a1 == ((a0 + 1) & 3))          /* a1 == 1 + a0 */
                continue;
            if (a1 == ((0 + 1) & 3))           /* a1 == 1 + sum_left (0) */
                continue;

            /* no NW constraint for the first column */

            int s0 = a0;                        /* sums of left for this column (none yet) */
            int s1 = a1;
            int idx = ((s0 << 2 | s1) << 4) | (a0 << 2 | a1);
            prev[idx] += (seq_t)1;
        }
    }

    /* ---- process remaining columns (col = 1 … n‑1) ---- */
    for (idx_t col = 1; col < n; ++col) {
        memset(next, 0, sizeof(next));

        for (int st = 0; st < 256; ++st) {
            seq_t ways = prev[st];
            if (ways == 0)
                continue;

            int s0      = (st >> 6) & 3;
            int s1      = (st >> 4) & 3;
            int a0prev  = (st >> 2) & 3;
            int a1prev  = st & 3;

            for (int a0 = 0; a0 < 4; ++a0) {
                /* top cell constraints */
                if (a0 == 1)
                    continue;                 /* forbids a0 == 1 (sum_above = 0) */
                if (a0 == ((s0 + 1) & 3))
                    continue;                 /* forbids a0 == 1 + sum_left_top */

                for (int a1 = 0; a1 < 4; ++a1) {
                    /* NE constraint from previous bottom cell */
                    if (a1prev == ((a0 + 1) & 3))
                        continue;                 /* a1prev == 1 + a0 invalid */

                    /* bottom cell constraints */
                    if (a1 == ((s1 + 1) & 3))
                        continue;                 /* a1 == 1 + sum_left_bottom */
                    if (a1 == ((a0 + 1) & 3))
                        continue;                 /* a1 == 1 + sum_above_bottom */
                    if (a1 == ((a0prev + 1) & 3))
                        continue;                 /* a1 == 1 + sum_NW_bottom */

                    int ns0 = (s0 + a0) & 3;
                    int ns1 = (s1 + a1) & 3;
                    int nidx = ((ns0 << 2 | ns1) << 4) | (a0 << 2 | a1);
                    next[nidx] += ways;
                }
            }
        }

        memcpy(prev, next, sizeof(prev));
    }

    /* ---- final column: NE sum is 0 → bottom cell cannot be 1 ---- */
    seq_t total = 0;
    for (int st = 0; st < 256; ++st) {
        seq_t ways = prev[st];
        if (ways == 0)
            continue;
        int lastBottom = st & 3;
        if (lastBottom == 1)            /* forbidden by NE (sum=0) */
            continue;
        total += ways;
    }

    return total;
}
