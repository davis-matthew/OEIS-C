/*
 * A162315  Triangular array 2*P - P^-1, where P is Pascal's triangle A007318.
 * https://oeis.org/A162315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (2 - (-1)^(r-k)) * C(r,k) where r = ⌊(√(8n+1)-1)/2⌋, k =
 *            n - r(r+1)/2, and C(r,k) is the binomial coefficient
 * Range:     u8 n <= 48, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128 values. */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0, hi = (idx_t)1ULL << 35;    /* 2^35 > sqrt(max) for all n */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Determine row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_u128(val);
    idx_t r = (s - 1) / 2;

    /* Start index of row r */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;

    /* Column index within row */
    idx_t k = n - start;

    /* Coefficient: 1 if (r-k) even, 3 if odd. */
    seq_t coeff = ((r - k) & 1ULL) ? 3 : 1;

    /* Binomial coefficient C(r,k) with 0 <= k <= r */
    if (k > r - k)   /* use symmetry to keep k small */
        k = r - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t mul = (seq_t)(r - k + i);
        res = res * mul / (seq_t)i;
    }

    return res * coeff;
}
