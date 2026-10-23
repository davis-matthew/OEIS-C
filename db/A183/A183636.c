/*
 * A183636  Number of (n+1) X 4 0..3 arrays with every 2 X 2 subblock summing
 *          to 6.
 * https://oeis.org/A183636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 110 + 100*2^n + 42*3^n + 4*4^n (valid for n ≥ 1)
 * Range:     u8 none, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Compute the powers once. */
    seq_t pow2n = pow2(n);   /* 2^n   */
    seq_t pow3n = pow3(n);   /* 3^n   */
    seq_t pow4n = pow4(n);   /* 4^n   */

    /* Assemble the closed‑form expression. */
    seq_t result = (seq_t)110
                 + (seq_t)100 * pow2n
                 + (seq_t)42  * pow3n
                 + (seq_t)4   * pow4n;

    return result;
}
