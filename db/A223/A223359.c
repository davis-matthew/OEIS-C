/*
 * A223359  Rolling cube footprints: number of 4 X n 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal or antidiagonal neighbor moves across a corresponding
 *          cube edge.
 * https://oeis.org/A223359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n arrays of digits 0..5 with the top‑left
 *            corner fixed to 0 and cube‑edge adjacency constraints.
 *            Recurrence for n > 7: a(n) = 144*a(n-1) - 3840*a(n-2) +
 *            24576*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values of the sequence (a(0) unused). */
    static const seq_t init[8] = {
        0ULL,
        216ULL,
        16384ULL,
        1769472ULL,
        191102976ULL,
        21177040896ULL,
        2356125106176ULL,
        262687716016128ULL
    };

    if (n <= 7)
        return init[n];

    /* a(n-3), a(n-2), a(n-1) for the current iteration. */
    seq_t a_minus3 = init[5];   /* a(5) */
    seq_t a_minus2 = init[6];   /* a(6) */
    seq_t a_minus1 = init[7];   /* a(7) */

    for (idx_t i = 8; i <= n; ++i) {
        seq_t cur = 144 * a_minus1 - 3840 * a_minus2 + 24576 * a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = cur;
    }

    return a_minus1;
}
