/*
 * A188790  Number of 7-step self-avoiding walks on an n X n X n X n 4-cube
 *          summed over all starting positions.
 * https://oeis.org/A188790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n>5, a(n) = 127160*n^4 - 654960*n^3 + 1132704*n^2 - 762240*n
 *            + 162048 Special cases: a(1)=0, a(2)=9984, a(3)=748128,
 *            a(4)=5886720, a(5)=22274448
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 14, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n <= 5) {
        switch ((idx_t)n) {
            case 1: return 0;
            case 2: return 9984;
            case 3: return 748128;
            case 4: return 5886720;
            case 5: return 22274448;
            default: return 0; /* unreachable */
        }
    }

    /* Compute the quartic polynomial using 128‑bit intermediates */
    unsigned __int128 val  = (unsigned __int128)127160 * n * n * n * n;
    val -= (unsigned __int128)654960 * n * n * n;
    val += (unsigned __int128)1132704 * n * n;
    val -= (unsigned __int128)762240 * n;
    val += 162048;

    return (seq_t)val;
}
