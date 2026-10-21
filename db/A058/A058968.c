/*
 * A058968  a(n) = 2^n + 2^(n - 1) - n - 8.
 * https://oeis.org/A058968
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 2^(n-1) - n - 8
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(3) is the first term; for n < 3 the value is undefined
       but we return 0 to keep the function well-typed. */
    if (n < 3)
        return 0;

    /* Compute 2^n and 2^(n-1) using the provided pow2 helper. */
    seq_t two_n   = pow2(n);
    seq_t two_n1  = pow2(n - 1);

    /* Convert the index to seq_t for arithmetic. */
    seq_t n_seq = (seq_t)n;

    /* Apply the formula. */
    return two_n + two_n1 - n_seq - 8;
}
