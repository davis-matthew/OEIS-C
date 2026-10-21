/*
 * A222993  Number of n X 2 0..2 arrays with successive rows and columns
 *          fitting to straight lines with nondecreasing slope, with a single
 *          point array taken as having zero slope.
 * https://oeis.org/A222993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-13 - 2^(6+n) + 3^(4+n) - 2*(1+2^(4+n))*n) / 4, with
 *            offset 1 (a(1) = 9). The function works for any seq_t and any
 *            idx_t type supplied by the build
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uintptr_t, if needed */

/* Fast exponentiation of seq_t by idx_t.  Works for all supported seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0)
    {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for smaller indices
       return 0 (undefined in OEIS). */
    if (n < 1)
        return (seq_t)0;

    /* Compute powers needed in the closed‑form expression. */
    seq_t pow2_6n = pow_seq((seq_t)2, n + 6);
    seq_t pow2_4n = pow_seq((seq_t)2, n + 4);
    seq_t pow3_4n = pow_seq((seq_t)3, n + 4);

    /* 2 * (1 + 2^(4+n)) * n */
    seq_t term = 2 * ((seq_t)1 + pow2_4n) * (seq_t)n;

    /* Numerator of the formula. */
    seq_t numerator = pow3_4n - pow2_6n - (seq_t)13 - term;

    /* Division by 4 – guaranteed to be exact for all n >= 1. */
    return numerator / (seq_t)4;
}
