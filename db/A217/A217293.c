/*
 * A217293  Permutation of natural numbers arising from applying the walk of
 *          right triangular type-3 spiral (defined in A214252) to the data of
 *          square spiral (e.g. A214526).
 * https://oeis.org/A217293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A217293 by simulating the right triangular type‑3 spiral walk
   and mapping the visited lattice point to its square‑spiral number. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    /* walk the right triangular type‑3 spiral */
    long long x = 0;
    long long y = 0;
    idx_t  steps   = n - 1;   /* number of remaining moves */
    long long seg_len = 1;    /* current segment length */
    int dir = 0;              /* 0: (-1,+1) diag; 1: (0,-1) down; 2: (+1,0) right */

    while (steps > 0) {
        idx_t use = seg_len <= steps ? seg_len : steps;
        if (dir == 0) {
            x -= (long long)use;
            y += (long long)use;
        } else if (dir == 1) {
            y -= (long long)use;
        } else { /* dir == 2 */
            x += (long long)use;
        }
        steps -= use;
        seg_len++;
        dir = (dir + 1) % 3;
    }

    /* map the lattice point (x,y) to the square‑spiral number */
    long long ax = x >= 0 ? x : -x;
    long long ay = y >= 0 ? y : -y;
    long long r  = ax > ay ? ax : ay;
    seq_t max_num = ((seq_t)2 * (seq_t)r + 1);
    max_num = max_num * max_num;  /* (2*r+1)^2 */

    if (y == -r) {                    /* bottom side */
        seq_t diff = (seq_t)(r - x);
        return max_num - diff;
    } else if (x == -r) {             /* left side */
        seq_t diff = (seq_t)(y + r);
        return max_num - 2 * (seq_t)r - diff;
    } else if (y == r) {              /* top side */
        seq_t diff = (seq_t)(x + r);
        return max_num - 4 * (seq_t)r - diff;
    } else {                          /* right side */
        seq_t diff = (seq_t)(r - y);
        return max_num - 6 * (seq_t)r - diff;
    }
}
