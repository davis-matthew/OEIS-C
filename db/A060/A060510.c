/*
 * A060510  Alternating with hexagonal stutters: if n is hexagonal (2k^2 - k,
 *          i.e., A000384) then a(n)=a(n-1), otherwise a(n) = 1 - a(n-1).
 * https://oeis.org/A060510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (n - floor((1 + sqrt(1 + 8*n)) / 4)) mod 2 Here a(0) = 0
 *            and for every n > 0, a(n) flips when n is not a hexagonal number
 *            (2k^2 - k)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of an __int128 value, floor(sqrt(x)). */
static inline unsigned long long isqrt_u128(unsigned __int128 x)
{
    /* Upper bound for sqrt(1 + 8*n) when n <= UINT64_MAX:
       sqrt(8*UINT64_MAX + 1) < 2^34, so 1<<36 is a safe high bound. */
    unsigned long long low = 0;
    unsigned long long high = (1ULL << 36);  /* 2^36 > required maximum */

    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;

        if (mid2 == x)
            return mid;
        if (mid2 < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;  /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Compute sqrt_floor(1 + 8*n) */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    unsigned long long s = isqrt_u128(val);

    /* Count of hexagonal numbers <= n */
    unsigned long long hexcnt = (1 + s) / 4;

    /* Number of non‑hexagonal numbers in [1 .. n] */
    unsigned long long diff = n - hexcnt;

    /* Return parity */
    return (seq_t)(diff & 1ULL);
}
