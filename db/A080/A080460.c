/*
 * A080460  a(1) = 2; for n > 1, a(n) = a(n-1) if n is already in the
 *          sequence, a(n) = a(n-1) + 4 otherwise.
 * https://oeis.org/A080460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 4 * ((n-2) - floor((n-2)/4))
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS A080460.
 * The sequence starts 2,2,6,10,14,14,...
 * The implementation uses the closed‑form expression
 * a(n) = 2 + 12*floor((n-2)/4) + 4*((n-2) mod 4),
 * which avoids any expensive search or bookkeeping. */
seq_t a(idx_t n)
{
    if (n <= 2u)
        return (seq_t)2;

    /* k = n-2 is guaranteed non‑negative */
    idx_t k = n - 2;
    idx_t q = k / 4;     /* floor((n-2)/4) */
    idx_t r = k % 4;     /* (n-2) mod 4 */

    /* Compute 2 + 12*q + 4*r using seq_t arithmetic. */
    seq_t res = (seq_t)2
              + (seq_t)12 * (seq_t)q
              + (seq_t)4  * (seq_t)r;
    return res;
}
