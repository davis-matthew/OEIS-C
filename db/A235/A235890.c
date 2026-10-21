/*
 * A235890  Number of (n+1) X (5+1) 0..3 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * ( 930 + 93 * 2^n + 4^n )
 * Range:     u8 none, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided pow_seq utility (exponentiation by squaring). */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* n must be >= 1 (sequence offset 1) */
    /* Compute 2^n */
    seq_t two_pow = pow_seq((seq_t)2, (int)n);
    /* 4^n = (2^n)^2, avoid a second costly pow_seq call */
    seq_t four_pow = two_pow * two_pow;

    /* a(n) = 4 * (930 + 93*2^n + 4^n) */
    seq_t t = (seq_t)930 + (seq_t)93 * two_pow + four_pow;
    return (seq_t)4 * t;
}
