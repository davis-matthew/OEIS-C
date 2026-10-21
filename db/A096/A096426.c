/*
 * A096426  Floor of the area consecutive odd sided triangles.
 * https://oeis.org/A096426
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( (2n+3)(2n-5)(2n-1)(6n-3) ) / 4 ) These
 *            factors come from (c+b-a)(a-c+b)(a+c-b)(a+b+c) for the
 *            consecutive odd‑sided triangle with sides a = 2n-3, b = 2n-1, c
 *            = 2n+1
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* integer square‑root for unsigned __int128 values */
static inline unsigned long long isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64);
    unsigned long long result = 0;
    while (low <= high) {
        unsigned __int128 mid   = (low + high) >> 1;
        unsigned __int128 midsq = mid * mid;
        if (midsq <= x) {
            result = (unsigned long long)mid;
            low    = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;             /* offset is 3 */

    /* For small seq_t types (≤ 32 bits) use 128‑bit arithmetic to avoid overflow. */
    if (sizeof(seq_t) <= 4) {
        unsigned __int128 d = (unsigned __int128)(2*n + 3);
        d *= (unsigned __int128)(2*n - 5);
        d *= (unsigned __int128)(2*n - 1);
        d *= (unsigned __int128)(6*n - 3);
        unsigned long long s = isqrt_uint128(d);   /* floor sqrt(D) */
        return (seq_t)(s / 4);
    }

    /* For larger seq_t types (≥ 64 bits or arbitrary precision) */
    seq_t f1 = 2*(seq_t)n + 3;   /* 2n+3 */
    seq_t f2 = 2*(seq_t)n - 5;   /* 2n-5 */
    seq_t f3 = 2*(seq_t)n - 1;   /* 2n-1 */
    seq_t f4 = 6*(seq_t)n - 3;   /* 6n-3 */

    seq_t D = f1 * f2;
    D *= f3;
    D *= f4;

    seq_t s = floor_sqrt(D);    /* integer square root of D */
    return s / 4;
}
