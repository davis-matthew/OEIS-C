/*
 * A060574  Tower of Hanoi: using the optimal way to move an even number of
 *          disks from peg 0 to peg 2 or an odd number from peg 0 to peg 1,
 *          a(n) is the smallest disk on peg 1 after n moves (or 0 if there
 *          are no disks on peg 1).
 * https://oeis.org/A060574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest disk on peg 1 after n moves of the optimal Tower
 *            of Hanoi solution that moves an even (odd) number of disks from
 *            peg 0 to peg 2 (peg 1). The function simulates the classic
 *            recursive algorithm by moving the smallest disc every step and,
 *            on even steps, moving the only legal disc oth...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Determine the number of disks needed so that the first n moves
       belong to a solution of that size.  D = floor(log2(n)) + 1. */
    int D = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        D++;
    }

    /* Peg stacks: 0..3 pegs, up to 64 disks (n fits in 64‑bit). */
    seq_t peg[3][64];
    int top[3] = {0, 0, 0};

    /* Initialise peg 0 with D discs, largest at the bottom. */
    top[0] = D;
    for (int i = 0; i < D; ++i)
        peg[0][i] = (seq_t)(D - i);   /* peg0[i] = D-i = disk number */

    int pos_disk1 = 0;           /* current peg of disk 1 */
    seq_t result = 0;

    for (idx_t step = 1; step <= n; ++step) {
        if (step & 1U) {  /* odd step: move disk 1 */
            int src = pos_disk1;
            int tgt = (src + 1) % 3;

            top[src]--;                       /* remove disk 1 */
            peg[tgt][top[tgt]] = 1;           /* put onto target */
            top[tgt]++;

            pos_disk1 = tgt;
        } else {  /* even step: move the only legal disk >1 */
            int a, b;
            if (pos_disk1 == 0) { a = 1; b = 2; }
            else if (pos_disk1 == 1) { a = 0; b = 2; }
            else { a = 0; b = 1; }

            int empty_a = (top[a] == 0);
            int empty_b = (top[b] == 0);
            seq_t disk_a = empty_a ? (seq_t)-1 : peg[a][top[a] - 1];
            seq_t disk_b = empty_b ? (seq_t)-1 : peg[b][top[b] - 1];

            if (empty_a) {  /* move b -> a */
                seq_t disk = peg[b][top[b] - 1];
                top[b]--;
                peg[a][top[a]] = disk;
                top[a]++;
            } else if (empty_b) {  /* move a -> b */
                seq_t disk = peg[a][top[a] - 1];
                top[a]--;
                peg[b][top[b]] = disk;
                top[b]++;
            } else if (disk_a < disk_b) {  /* a -> b */
                seq_t disk = peg[a][top[a] - 1];
                top[a]--;
                peg[b][top[b]] = disk;
                top[b]++;
            } else {  /* b -> a */
                seq_t disk = peg[b][top[b] - 1];
                top[b]--;
                peg[a][top[a]] = disk;
                top[a]++;
            }
        }

        /* Record the smallest disk on peg 1 after this move. */
        result = (top[1] == 0) ? 0 : peg[1][top[1] - 1];
    }

    return result;
}
