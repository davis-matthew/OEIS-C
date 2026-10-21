/*
 * A237591  Irregular triangle read by rows: T(n,k) is the difference between
 *          the total number of partitions of all positive integers <= n into
 *          exactly k consecutive parts, and the total number of partitions of
 *          all positive integers <= n into exactly k+1 consecutive parts
 *          (n>=1, 1<=k<=A003056(n)).
 * https://oeis.org/A237591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r and k are determined by the * flattened
 *            row‑major index n. T(r,k) = ceil((r+1)/k - (k+1)/2) * -
 *            ceil((r+1)/(k+1) - (k+2)/2). * The formula is computed with
 *            integer arithmetic, using a signed * ceil division for the
 *            second term where the numerator may be * negative
 * Range:     u8 n <= 1600, u16 n <= 1600, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Helpers:   ceil_div_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ceil_div_signed.h"

/* Integer square root: floor(sqrt(x)) for 0 ≤ x < 2^64. */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1ULL << 32;   /* 2^32 > sqrt(UINT64_MAX)   */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid == 0) {                      /* avoid x / 0 division      */
            low = mid + 1;
            continue;
        }
        if (mid <= x / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0 – safe fallback */

    /* Locate the row r that contains the n‑th flattened entry. */
    idx_t r   = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++r;
        idx_t len = (floor_sqrt_u64(8ULL * r + 1) - 1) / 2;
        cum += len;
    }

    /* Determine the column k within that row. */
    idx_t len = (floor_sqrt_u64(8ULL * r + 1) - 1) / 2;
    idx_t prev_cum = cum - len;
    idx_t k = n - prev_cum;            /* 1‑based column index */

    /* Compute T(r,k) using the closed formula. */
    uint64_t N = (uint64_t)r;
    uint64_t K = (uint64_t)k;

    /* A = ceil((N+1)/K - (K+1)/2) */
    int64_t numA = 2LL * (N + 1) - (int64_t)K * (K + 1);
    int64_t denA = 2LL * K;
    int64_t A    = ceil_div_signed(numA, denA);

    /* B = ceil((N+1)/(K+1) - (K+2)/2) */
    int64_t numB = 2LL * (N + 1) - (int64_t)(K + 1) * (K + 2);
    int64_t denB = 2LL * (K + 1);
    int64_t B    = ceil_div_signed(numB, denB);

    uint64_t result = (uint64_t)(A - B);   /* A >= B for all valid (r,k)  */
    return (seq_t)result;
}
