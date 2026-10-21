/*
 * A054401  a(n) = 5^n - 4^n - 1.
 * https://oeis.org/A054401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 4^n - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (offset 1, so a(1) = 0) */

#include "../utils/pow_seq.h"

/* Computes the n-th term of OEIS A054401: a(n) = 5^n - 4^n - 1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* sequence starts at n = 1 */

    /* Use the provided generic power routine; the exponent fits in int for
       all practical n considered here (n ≈ 10⁴ in the tests). */
    seq_t p5 = pow_seq((seq_t)5, (int)n);
    seq_t p4 = pow_seq((seq_t)4, (int)n);

    /* Subtract the two powers and one.  The unsigned arithmetic guarantees
       correct results as long as 5^n fits in seq_t; otherwise wrap‑around
       is acceptable because those values are beyond the build’s capacity. */
    return p5 - p4 - (seq_t)1;
}
