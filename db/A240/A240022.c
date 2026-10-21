/*
 * A240022  Total number of digits in palindromes with n digits.
 * https://oeis.org/A240022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10, if n = 1; a(n) = n * 9 * 10^(floor((n+1)/2)-1), if n
 *            > 1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper pow_uint from "../utils/pow_uint.h". */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Special case for n = 1. */
    if (n == 1)
        return (seq_t)10;

    /* Compute floor((n+1)/2) – 1 = (n-1)/2. */
    idx_t exp = (n + 1) / 2 - 1;

    /* 10^exp as seq_t. */
    seq_t pow10 = pow_uint(10, exp);

    /* Convert the index to seq_t for multiplication. */
    seq_t n_seq = (seq_t)n;

    /* a(n) = n * 9 * 10^exp. */
    return n_seq * (seq_t)9 * pow10;
}
