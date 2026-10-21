/*
 * A053731  a(n) = ceiling(binomial(n,8)/n).
 * https://oeis.org/A053731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( binomial(n,8) / n ) = ceil( (n-1)(n-2)…(n-7) /
 *            40320 )
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 112, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are defined by the build system (e.g., uint64_t, __int128, or BigInt). */

seq_t a(idx_t n)
{
    /* For n < 8 the binomial coefficient contains a factor zero, so a(n) = 0. */
    if (n <= 7) return 0;

    /* Compute the product (n-1)(n-2)…(n-7) in a wider type to avoid overflow. */
    __int128 prod = 1;
    for (idx_t k = 1; k <= 7; ++k)
        prod *= (int64_t)(n - k);          /* (n-k) fits in 64 bits for all n in tests */

    /* Denominator 8! = 40320 */
    const __int128 denom = 40320;
    /* Ceil division: (prod + denom - 1) / denom */
    __int128 result = (prod + denom - 1) / denom;

    return (seq_t)result;
}
