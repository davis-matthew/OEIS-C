/*
 * A183639  Number of (n+1) X 7 0..3 arrays with every 2 X 2 subblock summing
 *          to 6.
 * https://oeis.org/A183639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12138 + 483*2^(3+n) + 14*3^(3+n) + 4^(1+n)
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the closed‑form expression for sequence A183639. */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0, return 0
       (value is undefined, but this prevents potential division by zero
       or invalid indexing in user code). */
    if (n == 0) return 0;

    /* Compute the powers needed by the closed form. */
    seq_t term2 = pow2(n + 3);      /* 2^(3+n)   */
    seq_t term3 = pow3(n + 3);      /* 3^(3+n)   */
    seq_t term4 = pow4(n + 1);      /* 4^(1+n)   */

    /* Accumulate the result using seq_t arithmetic only. */
    seq_t result = (seq_t)12138;
    result += (seq_t)483 * term2;
    result += (seq_t)14  * term3;
    result += term4;

    return result;
}
