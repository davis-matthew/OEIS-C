/*
 * A093845  Row sums in A093844.
 * https://oeis.org/A093845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A110500(n)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    // Handle the offset: a(1) = 1
    if (n == 1) return (seq_t)1;

    seq_t base;

    // Check if n is a power of two
    if ((n & (n - 1)) == 0)
    {
        // Compute k = log2(n)
        idx_t k = 0;
        idx_t tmp = n;
        while (tmp > 1) { tmp >>= 1; ++k; }

        // For powers of two (except n==1) set the base:
        //   - if k >= 3 and k is odd → base = 6
        //   - otherwise                    → base = 2
        if (k >= 3 && (k & 1))
            base = (seq_t)6;
        else
            base = (seq_t)2;
    }
    else
    {
        // All other n: base is n itself
        base = (seq_t)n;
    }

    return big_pow(base, (seq_t)n);
}
