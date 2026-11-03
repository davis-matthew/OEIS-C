/*
 * A094527  Triangle T(n,k), read by rows, defined by T(n,k) =
 *          binomial(2*n,n-k).
 * https://oeis.org/A094527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(2*r, r-k) where r = floor((sqrt(8*n+1)-1)/2) and
 *            k = n - r*(r+1)/2
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 170, u64 n <= 594,
 *            u128 n <= 2210, big any n
 * Validated: standard, pass, 73 terms through n = 5139 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Use seq_t for all intermediate calculations to avoid overflow. */
    seq_t nseq = (seq_t)n;

    /* Compute s = floor_sqrt(8*n + 1). */
    seq_t x = ((seq_t)8 * nseq) + (seq_t)1;
    seq_t s = floor_sqrt(x);

    /* r = (s - 1) / 2 */
    seq_t rseq = (s - (seq_t)1) / (seq_t)2;

    /* prefix = r*(r+1)/2 */
    seq_t prefix = rseq * (rseq + (seq_t)1) / (seq_t)2;

    /* k = n - prefix */
    seq_t kseq = nseq - prefix;

    /* Convert to idx_t for the binomial arguments. */
    idx_t r = (idx_t)rseq;
    idx_t k = (idx_t)kseq;

    idx_t twor = r << 1;      /* 2 * r */
    idx_t rk   = r - k;       /* r - k */

    return binomial(twor, rk);
}
