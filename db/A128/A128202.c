/*
 * A128202  Configuration of discs on pegs after n steps of the optimal
 *          solution to the Towers of Hanoi problem moving an odd number of
 *          discs from peg 0 to peg 2, or an even number from peg 0 to peg 1.
 * https://oeis.org/A128202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = configuration after n-1 steps of the optimal Towers of
 *            Hanoi solution moving an odd number of discs from peg 0 to 2 or
 *            an even number from 0 to 1, encoded in base‑3 with the least
 *            significant digit for the smallest disc
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   hanoi_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/hanoi_rec.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  a(1) corresponds to the initial configuration (no moves). */
    idx_t steps = n - 1;                    /* 0‑based step number */
    if (steps == 0) return (seq_t)0;

    /* N = number of disks needed to cover 'steps' moves: 2^N-1 >= steps. */
    int N = 0;
    idx_t t = steps;
    while (t) { N++; t >>= 1; }

    unsigned char pos[64] = {0};            /* temporary storage for disk pegs */

    int dst = (N & 1) ? 2 : 1;              /* target peg according to parity */
    int aux = 3 - dst;                      /* auxiliary peg (0 + 1 + 2 = 3) */

    hanoi_rec(N, 0, dst, aux, steps, pos);

    /* Convert the peg array to a base‑3 integer. */
    seq_t result = 0;
    seq_t power = 1;                        /* 3^i for i=0..N-1 */
    for (int i = 0; i < N; ++i) {
        result += (seq_t)pos[i] * power;
        power *= 3;
    }
    return result;
}
