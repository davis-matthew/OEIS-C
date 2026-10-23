/*
 * A187854  Number of 6-step king-knight's tours (piece capable of both kinds
 *          of moves) on an n X n board summed over all starting positions.
 * https://oeis.org/A187854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 706880*n^2 - 5180252*n + 9274644 for n > 12. For n <= 12
 *            use the precomputed values
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[13] = {
    0ULL,            /* dummy for index 0 */
    0ULL,            /* a(1) */
    0ULL,            /* a(2) */
    16032ULL,        /* a(3) */
    292776ULL,       /* a(4) */
    1400168ULL,      /* a(5) */
    3807828ULL,      /* a(6) */
    7700944ULL,      /* a(7) */
    13082348ULL,     /* a(8) */
    19910456ULL,     /* a(9) */
    28160124ULL,     /* a(10) */
    37824352ULL,     /* a(11) */
    48902340ULL      /* a(12) */
};

seq_t a(idx_t n) {
    if (n <= 12) {
        return init[(size_t)n];
    }
    seq_t nn = (seq_t)n;
    return 706880 * nn * nn - 5180252 * nn + 9274644;
}
