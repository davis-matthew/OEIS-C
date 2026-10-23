/*
 * A198193  Replace 2^k in the binary representation of n with n+(k-L) where L
 *          = floor(log(n)/log(2)).
 * https://oeis.org/A198193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n-L)*A000120(n) + A073642(n) where L = floor(log₂(n)),
 *            A000120(n) = popcount(n), A073642(n) = sum of the exponents of
 *            the powers of 2 in the binary expansion of n
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial zero case */
    if (n == 0)
        return 0;

    /* Number of set bits (popcount) */
    idx_t pop = (idx_t)__builtin_popcountll((unsigned long long)n);

    /* Sum of the indices of set bits */
    idx_t sumk = 0;
    idx_t temp = n;
    for (idx_t k = 0; temp; ++k) {
        if (temp & 1)
            sumk += k;
        temp >>= 1;
    }

    /* Most significant bit position L = floor(log2(n)) */
    idx_t L = 63 - (idx_t)__builtin_clzll((unsigned long long)n);

    /* Compute (n - L) * popcount */
    seq_t term1 = ((seq_t)pop) * (((seq_t)n) - (seq_t)L);

    /* Final result */
    return term1 + (seq_t)sumk;
}
