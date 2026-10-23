/*
 * A135827  Moves for corner 3-cycles (brd,urb,ulb) in a 3 X 3 X 3 Rubik's
 *          Cube.
 * https://oeis.org/A135827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 3*11^(n+1) for even n * a(n) = 27*7^n for odd n * This
 *            implements OEIS sequence A135827
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {              /* n even */
        seq_t exp = (seq_t)(n + 1);   /* exponent = n+1 */
        seq_t res = big_pow((seq_t)11, exp);
        return 3 * res;
    } else {                          /* n odd */
        seq_t exp = (seq_t)n;         /* exponent = n */
        seq_t res = big_pow((seq_t)7, exp);
        return 27 * res;
    }
}
