/*
 * A036070  Expansion of (-1+1/(1-4*x)^4)/(16*x); related to A038846.
 * https://oeis.org/A036070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^(n-1) * binomial(n+4, 3) With a(0) = 1 (since 4^(−1) ·
 *            binomial(4,3) = 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* binomial(n+4,3) is the combinatorial factor */
    seq_t bin = binomial(n + 4, 3);
    /* 4^(n-1) computed as 4^(n-1) = 2^(2*(n-1)) */
    seq_t pw  = pow_int((seq_t)4, n - 1);
    return bin * pw;
}
