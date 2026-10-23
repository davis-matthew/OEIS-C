/*
 * A120845  a(n) = 2^n + 3^n + 5*n.
 * https://oeis.org/A120845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 3^n + 5*n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=2, a(1)=10, a(2)=23, a(3)=50, a(4)=117, ... */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute 2^n and 3^n using the supplied utilities */
    seq_t p2 = pow2(n);
    seq_t p3 = pow3(n);

    /* Compute 5*n in seq_t domain */
    seq_t mul = (seq_t)5 * (seq_t)n;

    /* Return the sum */
    return p2 + p3 + mul;
}
