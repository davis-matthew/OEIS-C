/*
 * A127847  a(n)=4^C(n,2)*(4^n-1)/3.
 * https://oeis.org/A127847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^C(n,2)*(4^n-1)/3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases separately to avoid an unsigned underflow
       when computing n*(n-1)/2 for n = 0. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Compute the exponent C(n,2) = n*(n-1)/2 safely as an unsigned
       integer. */
    idx_t expC = n * (n - 1) / 2;

    /* 4^C(n,2) and 4^n.  The function big_pow returns a seq_t value
       raised to the given seq_t exponent. */
    seq_t pow4C = big_pow((seq_t)4, (seq_t)expC);
    seq_t pow4n = big_pow((seq_t)4, (seq_t)n);

    /* (4^n - 1) / 3, integral by construction. */
    seq_t temp = pow4n - 1;
    temp /= 3;

    /* Final result. */
    return pow4C * temp;
}
