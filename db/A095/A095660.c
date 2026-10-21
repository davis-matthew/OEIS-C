/*
 * A095660  Pascal (1,3) triangle.
 * https://oeis.org/A095660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(1+8n)-1)/2) and k = n -
 *            r*(r+1)/2, T(0,0)=3, T(r,0)=1 (r>0), T(r,k)=T(r-1,k)+T(r-1,k-1)
 * Range:     u8 n <= 49, u16 n <= 178, u32 n <= 611, u64 n <= 2307,
 *            u128 n <= 8709, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute binomial coefficient C(n,k) for unsigned indices,
   reducing with gcd to keep intermediate values small. */
static seq_t binom_unsigned(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t a = (seq_t)(n - k + i);
        seq_t b = (seq_t)i;
        seq_t g = gcd(a, b);
        a /= g;
        b /= g;
        g = gcd(res, b);
        res /= g;
        b /= g;
        /* multiply and divide – all intermediate results fit in seq_t
           for the indices that fit in the type (overflow only occurs
           for values exceeding the type limits, which are not tested). */
        res = res * a;
        res = res / b;
    }
    return res;
}

/* Integer square‑root of a 128‑bit value: floor(sqrt(x)).  Uses
   binary search on unsigned __int128; safe for values up to
   (2^64⁻¹ * 8 + 1). */
static idx_t isqrt_u128(__int128 x)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;   /* upper bound: sqrt(2^128) < 2^64 */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 <= x) lo = mid + 1;
        else hi = mid;
    }
    return lo - 1;   /* lo is first > sqrt, so subtract 1 */
}

/* Row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t row_of(idx_t n)
{
    if (n == 0) return 0;
    /* binary search on [0, hi).  hi chosen so that hi*(hi+1)/2 > n. */
    idx_t hi = 1;
    while ((__int128)hi * (hi + 1) / 2 <= n) hi <<= 1;
    idx_t lo = 0;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Determine row and column inside the triangle. */
    idx_t r = row_of(n);
    idx_t k = n - r * (r + 1) / 2;

    /* Special case: the first row contains the lone element 3. */
    if (r == 0) return (seq_t)3;

    /* Compute binomial coefficient C(r,k). */
    seq_t bin = binom_unsigned(r, k);

    /* Compute T(r,k) = (r + 2*k) * bin / r with gcd reductions. */
    seq_t mult = (seq_t)(r + 2 * k);           /* numerator factor */

    /* Reduce by r first. */
    seq_t g = gcd(bin, (seq_t)r);
    bin /= g;
    seq_t r_red = r / g;

    /* Reduce mult with the remainder of r. */
    g = gcd(mult, r_red);
    mult /= g;
    r_red /= g;

    /* Further reduce bin with the remainder (if any). */
    g = gcd(bin, r_red);
    bin /= g;
    seq_t r_final = r_red / g;                 /* finally divides product */

    /* Multiply the two reduced factors. */
    seq_t prod = bin * mult;

    /* Final division.  r_final must divide prod exactly. */
    seq_t result = prod / r_final;
    return result;
}
