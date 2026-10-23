/*
 * A033463  EXPCONV of squares A000290 with themselves.
 * https://oeis.org/A033463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-4)*(n^4+6*n^3+19*n^2+22*n+16)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 45, u128 n <= 105,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    seq_t poly = n4
               + (seq_t)6  * n3
               + (seq_t)19 * n2
               + (seq_t)22 * nn
               + (seq_t)16;

    if (n < 4) {
        int diff = 4 - (int)n;        /* non‑negative for n = 0..3 */
        return poly >> diff;
    }
    /* n >= 4 : multiply by 2^(n-4) */
    return poly * pow2(n - 4);
}
