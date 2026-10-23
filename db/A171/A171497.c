/*
 * A171497  a(n) = 2*a(n-1) for n > 1; a(0) = 3, a(1) = 8.
 * https://oeis.org/A171497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3, a(1) = 8; a(n) = 2*a(n-1) for n > 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses pow_int from utils to compute 2^n efficiently. */
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    /* For n > 0: a(n) = 4 * 2^n  (i.e., 2^(n+2)) */
    seq_t pow2 = pow_int((seq_t)2, n);
    return (seq_t)(pow2 * ((seq_t)4));
}
