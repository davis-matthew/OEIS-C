/*
 * A183634  Number of (n+1) X 2 0..3 arrays with every 2 x 2 subblock summing
 *          to 6.
 * https://oeis.org/A183634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 2^(2+n) + 2*3^(1+n) + 4^(1+n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the closed form derived by Christian Krause (2018). */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* n is 1-indexed.  All operations are done in seq_t. */
    seq_t result = 2;                       /* constant term */

    /* 2^(2+n)  ->  pow2(n+2) */
    result += pow2(n + 2);

    /* 2*3^(1+n) -> 2 * pow3(n+1) */
    result += 2 * pow3(n + 1);

    /* 4^(1+n) -> pow4(n+1) */
    result += pow4(n + 1);

    return result;
}
