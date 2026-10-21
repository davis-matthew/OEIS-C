/*
 * A213247  Number of nonzero elements in GF(2^n) that are 11th powers.
 * https://oeis.org/A213247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1) / gcd(2^n - 1, 11)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently, a(n) =  (2^n - 1)/11  if 10 | n, else 2^n - 1) */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; n==0 is unused but handled */
        return 0;

    seq_t two_to_n = pow2(n);
    seq_t M = two_to_n - ((seq_t)1);

    /* When n is a multiple of 10, 2^n – 1 is divisible by 11. */
    if ((n % (idx_t)10) == 0)
        return M / ((seq_t)11);
    else
        return M;
}
