/*
 * A073378  Eighth convolution of A001045(n+1) (generalized (1,2)-Fibonacci),
 *          n>=0, with itself.
 * https://oeis.org/A073378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 40, u128 n <= 95,
 *            big any n
 * Helpers:   my_binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A073378
 * Formula: a(n) = Σ_{k=0..⌊n/2⌋} binomial(n-k+8,8) * binomial(n-k,k) * 2^k
 * (OEIS offset 0)
 */

#include "../utils/pow2.h"
#include "../utils/my_binom.h"

seq_t a(idx_t n)
{
    seq_t ans = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = my_binom(n - k + 8, 8);
        seq_t t2 = my_binom(n - k, k);
        seq_t t3 = pow2(k);
        seq_t term = t1;
        term = term * t2;
        term = term * t3;
        ans = ans + term;
    }
    return ans;
}
