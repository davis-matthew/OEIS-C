/*
 * A189521  n+[ns/r]+[nt/r]; r=1, s=1/(e-1), t=1/(e+1).
 * https://oeis.org/A189521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/(e-1)⌋ + ⌊n/(e+1)⌋ (r=1, s=1/(e-1), t=1/(e+1))
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t FACT[21] = {
    1ULL,
    1ULL,
    2ULL,
    6ULL,
    24ULL,
    120ULL,
    720ULL,
    5040ULL,
    40320ULL,
    362880ULL,
    3628800ULL,
    39916800ULL,
    479001600ULL,
    6227020800ULL,
    87178291200ULL,
    1307674368000ULL,
    20922789888000ULL,
    355687428096000ULL,
    6402373705728000ULL,
    121645100408832000ULL,
    2432902008176640000ULL
};

seq_t a(idx_t n)
{
    /* D = 20! */
    const uint64_t D = FACT[20];

    /* Compute P = sum_{k=1}^{20} D/k!  (integer) */
    __int128 P = 0;
    for (int k = 1; k <= 20; ++k) {
        P += (__int128)D / FACT[k];
    }

    /* Denominator for e+1 term: P + 2*D  */
    __int128 D2 = (__int128)D;
    __int128 denom2 = P + 2 * D2;

    __int128 nn = ( __int128 ) n;

    /* t1 = ⌊ n*D / P ⌋  */
    __int128 t1 = (nn * D2) / P;

    /* t2 = ⌊ n*D / (P+2D) ⌋  */
    __int128 t2 = (nn * D2) / denom2;

    /* Result in the integer type seq_t */
    seq_t result = (seq_t)n + (seq_t)t1 + (seq_t)t2;
    return result;
}
