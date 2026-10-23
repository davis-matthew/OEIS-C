/*
 * A028777  Nonsquares mod 64.
 * https://oeis.org/A028777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th positive integer that is not a quadratic residue
 *            modulo 64 (i.e., not in {0,1,4,9,16,17,25,33,36,41,49,57})
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Squares modulo 64 (1 if r is a quadratic residue, 0 otherwise). */
    static const unsigned char squares[64] = {
        1, /* 0   */
        1, /* 1   */
        0, /* 2   */
        0, /* 3   */
        1, /* 4   */
        0, /* 5   */
        0, /* 6   */
        0, /* 7   */
        0, /* 8   */
        1, /* 9   */
        0, /*10   */
        0, /*11   */
        0, /*12   */
        0, /*13   */
        0, /*14   */
        0, /*15   */
        1, /*16   */
        1, /*17   */
        0, /*18   */
        0, /*19   */
        0, /*20   */
        0, /*21   */
        0, /*22   */
        0, /*23   */
        0, /*24   */
        1, /*25   */
        0, /*26   */
        0, /*27   */
        0, /*28   */
        0, /*29   */
        0, /*30   */
        0, /*31   */
        0, /*32   */
        1, /*33   */
        0, /*34   */
        0, /*35   */
        1, /*36   */
        0, /*37   */
        0, /*38   */
        0, /*39   */
        0, /*40   */
        1, /*41   */
        0, /*42   */
        0, /*43   */
        0, /*44   */
        0, /*45   */
        0, /*46   */
        0, /*47   */
        0, /*48   */
        1, /*49   */
        0, /*50   */
        0, /*51   */
        0, /*52   */
        0, /*53   */
        0, /*54   */
        0, /*55   */
        0, /*56   */
        1, /*57   */
        0, /*58   */
        0, /*59   */
        0, /*60   */
        0, /*61   */
        0, /*62   */
        0  /*63   */
    };

    idx_t count = 0;
    seq_t cand = 0;

    while (count < n) {
        cand++;
        unsigned int r = (unsigned int)(cand % 64); /* cand mod 64 */
        if (!squares[r]) {
            count++;
        }
    }

    return cand;
}
