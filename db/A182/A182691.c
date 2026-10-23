/*
 * A182691  Composite Beatty sequence of sqrt(2).
 * https://oeis.org/A182691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3; a(n)=floor((2+sqrt(2))*a(n-1)) if n is odd;
 *            a(n)=floor(sqrt(2)*a(n-1)) if n is even
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 29,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Helper: floor(sqrt(2) * x) without floating point.
   For 64‑bit or less seq_t it uses 128‑bit arithmetic.
   For larger seq_t (arbitrary precision) it forms 2*x*x and
   applies floor_sqrt. */
static seq_t floortwo_sqrt2_times(seq_t x)
{
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* 128‑bit intermediate: 2*x*x */
        unsigned __int128 prod = (unsigned __int128)2 *
                                 (unsigned __int128)x *
                                 (unsigned __int128)x;
        unsigned __int128 low = 0;
        unsigned __int128 high = ((unsigned __int128)1 << 64);
        while (low < high) {
            unsigned __int128 mid = (low + high) >> 1;
            unsigned __int128 sq = mid * mid;
            if (sq <= prod)
                low = mid + 1;
            else
                high = mid;
        }
        return (seq_t)(low - 1);
    } else {
        seq_t prod = (seq_t)2 * x * x;
        return floor_sqrt(prod);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* outside the offset */
    seq_t cur = (seq_t)3;          /* a(1) */
    if (n == 1) return cur;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t t = floortwo_sqrt2_times(cur);
        if ((i & 1) == 0) {       /* even index – multiply by sqrt(2) */
            cur = t;
        } else {                  /* odd index – multiply by (2+sqrt(2)) */
            cur = cur * (seq_t)2 + t;
        }
    }
    return cur;
}
