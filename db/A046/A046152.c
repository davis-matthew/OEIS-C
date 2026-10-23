/*
 * A046152  a(n) = n*phi(n) + 1.
 * https://oeis.org/A046152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*phi(n)+1
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* phi is a helper that returns ϕ(n) as a seq_t value */
    seq_t ph = phi(n);
    return (seq_t)n * ph + 1;
}
