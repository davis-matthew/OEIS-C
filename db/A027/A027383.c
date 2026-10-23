/*
 * A027383  a(2*n) = 3*2^n - 2; a(2*n+1) = 2^(n+2) - 2.
 * https://oeis.org/A027383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(2n) = 3*2^n - 2, a(2n+1) = 2^(n+2) - 2
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 125,
 *            u128 n <= 253, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the supplied pow2(idx_t) routine for powers of two. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Even index: n = 2*k */
    if ((n & 1ULL) == 0ULL) {
        idx_t k = n >> 1;
        seq_t pow = pow2(k);          /* 2^k          */
        seq_t three = 3;
        seq_t two   = 2;
        return three * pow - two;     /* 3*2^k - 2   */
    }
    /* Odd index: n = 2*k + 1 */
    else {
        idx_t k = (n - 1ULL) >> 1;    /* floor(n/2)   */
        seq_t pow = pow2(k + 2);      /* 2^(k+2)      */
        seq_t two = 2;
        return pow - two;             /* 2^(k+2) - 2 */
    }
}
