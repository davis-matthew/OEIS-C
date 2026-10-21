/*
 * A235892  Number of (n+1) X (7+1) 0..3 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * (16002 + 381 * 2^n + 4^n)
 * Range:     u8 none, u16 none, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the supplied pow2 utility to compute 2^exp as a seq_t. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset 1: undefined for n < 1 */

    /* Compute 2^n and 4^n (= 2^(2n)) using the helper pow2. */
    seq_t pow2n = pow2(n);
    seq_t pow4n = pow2(2 * n);          /* 4^n = 2^(2n) */

    /* Assemble the closed‑form expression, step by step. */
    seq_t result = pow4n;               /* start with 4^n */
    result += (seq_t)381 * pow2n;       /* add 381·2^n */
    result += (seq_t)16002;             /* add 16002 */
    result *= 4;                        /* multiply the whole sum by 4 */

    return result;
}
