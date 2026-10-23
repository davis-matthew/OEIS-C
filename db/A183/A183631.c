/*
 * A183631  Number of (n+1) X 9 0..2 arrays with every 2 X 2 subblock summing
 *          to 4.
 * https://oeis.org/A183631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(6220 + 85*2^(n+2) + 3^n)
 * Range:     u8 none, u16 n <= 5, u32 n <= 19, u64 n <= 39, u128 n <= 66,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses pow_int from the utilities for arbitrary‑precision computation. */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* 2^(n+2) */
    seq_t two  = pow_int((seq_t)2, (idx_t)(n + 2));
    /* 3^n */
    seq_t three = pow_int((seq_t)3, n);
    /* a(n) = 3 * (6220 + 85*two + three) */
    seq_t term  = (seq_t)6220 + (seq_t)85 * two + three;
    return (seq_t)3 * term;
}
