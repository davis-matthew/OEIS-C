/*
 * A229215  If 1, 2, and 3 represent the three 2D vectors (1,0),
 *          (0.5,sqrt(3)/2) and (-0.5,sqrt(3)/2) and -1, -2 and -3 are the
 *          negation of these vectors, then this sequence represents Gosper's
 *          island.
 * https://oeis.org/A229215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 9925, i16 n <= 9925, i32 n <= 9925, i64 n <= 9925,
 *            i128 n <= 9925, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) of OEIS A229215.
    The sequence is defined by the substitution
        P(1) = 1, -3, 1
        P(2) = 2,  1, 2
        P(3) = 3,  2, 3
      with P(-i) = -P(i).
    The seed symbol is 1.
    The nth term is obtained by interpreting n‑1 in base 3
    and applying the mapping in the order of the most
    significant digit to the least.                                       */

#include <stdint.h>

static const seq_t map_pos[4][3] = {
    {0, 0, 0},            /* unused index 0 */
    {1, -3, 1},           /* P(1) */
    {2,  1, 2},           /* P(2) */
    {3,  2, 3}            /* P(3) */
};

seq_t a(idx_t n)
{
    /* n >= 1 guarantees that n-1 does not underflow */
    idx_t idx = n - 1;

    /* store base‑3 digits of idx (least significant first) */
    int rems[65];
    int depth = 0;
    while (idx > 0) {
        rems[depth++] = (int)(idx % 3);
        idx /= 3;
    }

    /* start from the seed symbol 1 */
    seq_t val = 1;

    /* apply the mapping from most significant digit to least  */
    for (int d = depth - 1; d >= 0; --d) {
        int r = rems[d];
        if (val > 0)
            val = map_pos[(int)val][r];
        else
            val = -map_pos[-(int)val][r];
    }

    return val;
}
