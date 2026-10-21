/*
 * A084636  Binomial transform of (1,0,1,0,1,0,2,0,2,0,2,0,...).
 * https://oeis.org/A084636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n - (C(n,0) + C(n,2) + C(n,4)) (C(n,k) is
 *            binomial(n,k))
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Computes the n-th term of OEIS sequence A084636.
   The function has external linkage and works with any seq_t
   (unsigned integer type or arbitrary‑precision integer). */
seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^n */
    seq_t pow2n = pow2(n);

    /* Compute the corrective sum: C(n,0)+C(n,2)+C(n,4) */
    seq_t sub = (seq_t)1;                     /* C(n,0) = 1 */
    if (n >= 2)
        sub += binomial(n, 2);                /* C(n,2) */
    if (n >= 4)
        sub += binomial(n, 4);                /* C(n,4) */

    /* a(n) = 2^n - sub */
    return pow2n - sub;
}
