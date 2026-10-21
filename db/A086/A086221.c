/*
 * A086221  Bisection of A086652.
 * https://oeis.org/A086221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2n+2) - 3*2^(n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"          /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       only to avoid undefined behaviour from the caller. */
    if (n == 0)
        return 0;

    /* Exponents are small enough to fit in idx_t (uint64_t). */
    seq_t exp_big   = (seq_t)(2*n + 2);   /* exponent 2n+2  */
    seq_t exp_small = (seq_t)(n - 1);     /* exponent n-1   */

    /* Compute the powers of two using big_pow so the code works
       for arbitrary‑precision seq_t as well as for the built‑in
       integer types.  For the built‑in types the multiplication
       steps stay within the width for all n that fit in seq_t. */
    seq_t pow_big   = big_pow((seq_t)2, exp_big);
    seq_t pow_small = big_pow((seq_t)2, exp_small);

    /* Assemble the term: 2^(2n+2) - 3*2^(n-1) */
    return pow_big - (seq_t)3 * pow_small;
}
