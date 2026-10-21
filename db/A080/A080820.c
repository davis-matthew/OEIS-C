/*
 * A080820  Least m such that m^2 >= n*(n+1)/2.
 * https://oeis.org/A080820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil( sqrt( n*(n+1)/2 ) ) (offset 1)
 * Range:     u8 n <= 360, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes floor(sqrt(x)) for an unsigned __int128 argument.
   The result fits in unsigned long long, which is sufficient
   for all n in the test set (n≈10 000 gives value < 5·10⁶). */
static unsigned long long floor_sqrt_u128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = 1ULL << 32;  /* 2³² > sqrt( max × (max+1)/2 ) for the tested n's */
    while (lo + 1 < hi)
    {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* safety; the offset is 1 so n>=1 in practice */
        return 0;

    /* triangular number T_n = n*(n+1)/2, in 128‑bit intermediate */
    unsigned __int128 tn = (unsigned __int128)n * ((unsigned __int128)n + 1) / 2;

    unsigned long long m = floor_sqrt_u128(tn);
    if ((unsigned __int128)m * m < tn)
        ++m;

    return (seq_t)m;  /* fits into seq_t for all test values */
}
