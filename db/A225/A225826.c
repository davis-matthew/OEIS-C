/*
 * A225826  Number of binary pattern classes in the (2,n)-rectangular grid:
 *          two patterns are in same class if one of them can be obtained by a
 *          reflection or 180-degree rotation of the other.
 * https://oeis.org/A225826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3) * (2^(n+1) - (-1)^n + 7) with a(0) = 1, a(1) = 3,
 *            a(2) = 7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the small initial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;

    /* For n >= 3 use the closed‑form formula */
    seq_t p1 = pow2(n - 3);      /* 2^(n-3) */
    seq_t p2 = pow2(n + 1);      /* 2^(n+1) */

    /* Add the constant part depending on parity of n */
    seq_t term;
    if ((n & 1) == 0) {           /* n even */
        term = p2 + (seq_t)6;
    } else {                      /* n odd */
        term = p2 + (seq_t)8;
    }

    return p1 * term;
}
