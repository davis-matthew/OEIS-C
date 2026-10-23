/*
 * A132774  A natural number operator.
 * https://oeis.org/A132774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let k be the smallest integer with k(k+1)/2 >= n. Then a(n) =
 *            2*k-1 if n = k(k+1)/2; = 2*(k-1) if n = k(k+1)/2 - 1; = 0
 *            otherwise. (k >= 1)
 * Range:     u8 n <= 8383, u16 n <= 8941, u32 n <= 8941, u64 n <= 8941,
 *            u128 n <= 8941, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline __uint128_t triangular(idx_t k)
{
    return (__uint128_t)k * (k + 1) / 2;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t low = 0;
    idx_t high = 1;
    while (triangular(high) < (__uint128_t)n)
        high <<= 1;                     /* ensure high is large enough */

    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= (__uint128_t)n)
            high = mid;
        else
            low = mid;
    }

    idx_t k = high;
    __uint128_t tk = triangular(k);

    if ((__uint128_t)n == tk)
        return (seq_t)((seq_t)2 * (seq_t)k - 1);
    if ((__uint128_t)n == tk - 1)
        return (seq_t)((seq_t)2 * (seq_t)(k - 1));

    return 0;
}
