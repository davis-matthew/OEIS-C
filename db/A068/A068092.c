/*
 * A068092  Index of smallest triangular number with n digits.
 * https://oeis.org/A068092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = index of smallest triangular number with n digits * a(n)
 *            = floor_sqrt(2*10^(n-1)) rounded up, * i.e. b =
 *            floor(sqrt(2*10^(n-1))) and if b(b+1)/2 < 10^(n-1) then b++
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 48,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

/* Integer square root for 128‑bit values.
 * Returns floor(sqrt(x)) for x >= 0. */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1; /* 2^64‑1 is the max sqrt */
    unsigned __int128 ans = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0u) return 0;          /* offset is 1 – unused for n=0 */

    /* If seq_t fits in 8 bytes or less, guard against overflow when
     * 10^(n-1) or 2*10^(n-1) cannot be represented in seq_t.          */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        /* Compute 10^(n-1) as unsigned __int128 to avoid overflow. */
        unsigned int e = (unsigned int)(n - 1u);
        unsigned __int128 ten = 1;
        for (unsigned int i = 0; i < e; ++i)
            ten *= 10;

        unsigned __int128 target = ten * 2;
        unsigned __int128 b = isqrt128(target);
        unsigned __int128 tri = b * (b + 1) / 2;
        if (tri < ten) ++b;
        return (seq_t)b;                /* b fits in seq_t for n <= 39 */
    }

    /* Arbitrary‑precision case: seq_t is larger than 64 bits or a
     * big integer class.  Use the utilities that handle large values. */
    seq_t ten_pow = pow10(n - 1);
    seq_t target = (seq_t)2 * ten_pow;
    seq_t b = floor_sqrt(target);
    seq_t t = (b * (b + (seq_t)1)) / (seq_t)2;
    if (t < ten_pow) ++b;
    return b;
}
