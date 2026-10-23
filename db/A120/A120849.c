/*
 * A120849  a(n) = 5*n + 3^n - 2^n.
 * https://oeis.org/A120849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n + 3^n - 2^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

/* Compute the nth term of OEIS A120849.
 * Sequence definition: a(n) = 5*n + 3^n - 2^n,   a(0) = 0.
 * The function uses only integer arithmetic and the helper
 * pow_uint which returns a (seq_t) value of base^exp. */
seq_t a(idx_t n)
{
    /* Compute 3^n and 2^n as seq_t values. */
    seq_t pow3n = pow_uint(3, n);
    seq_t pow2n = pow_uint(2, n);

    /* a(n) = (3^n - 2^n) + 5*n.  The subtraction is safe because
     * 3^n >= 2^n for all n >= 0. */
    seq_t diff    = pow3n - pow2n;
    seq_t five_n  = (seq_t)n * 5;

    return diff + five_n;
}
