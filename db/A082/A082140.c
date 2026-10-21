/*
 * A082140  A transform of binomial(n,6).
 * https://oeis.org/A082140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(binomial(n+6,6) * 2^(n-1))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 41, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for all n >= 0.  The function works for any
   seq_t type that supports the usual arithmetic operators
   (+, -, *, /, <<, >>) and has an integer literal constructor.
   It uses only simple integer arithmetic and is O(1). */
seq_t a(idx_t n)
{
    /* Special case n = 0 : 2^(n-1) = ½,  ceil(C(6,6) * ½) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute C(n+6, 6) multiplicatively: */
    seq_t bin = (seq_t)1;
    for (idx_t i = 1; i <= 6; ++i) {
        bin = bin * (n + i); /* multiply by (n + i) */
        bin = bin / i;       /* divide by i (exact division) */
    }

    /* Compute 2^(n-1) as a left shift of 1.  This works for both
       fixed‑size and arbitrary‑precision seq_t. */
    seq_t pow2 = (seq_t)1 << (n - 1);

    /* Result is the product of the two terms. */
    return bin * pow2;
}
