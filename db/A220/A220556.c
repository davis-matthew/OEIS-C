/*
 * A220556  Square array T(n,k) = ((n+k-1)*(n+k-2)/2+n)^k, n,k > 0 read by
 *          antidiagonals.
 * https://oeis.org/A220556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n^((t^2 + 3t + 4)/2 - n), where * t = floor((-1 + sqrt(8n
 *            - 7))/2). * This is equivalent to the antidiagonal enumeration
 *            of * T(n,k) = ((n+k-1)(n+k-2)/2 + n)^k
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 28, u64 n <= 66, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root of a 128‑bit value (non‑negative).
 * Returns floor(sqrt(x)). */
static inline idx_t isqrt_u128(__int128 x)
{
    idx_t low = 0, high = (idx_t)1 << 32;          /* 2^32 > sqrt(2^66) */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __int128 mm = (__int128)mid * mid;
        if (mm <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute a(n).  n >= 1.  The result is exact when it fits in seq_t. */
seq_t a(idx_t n)
{
    /* Compute t = floor((-1 + sqrt(8n - 7))/2). */
    __int128 tmp = (__int128)8 * n - 7;
    idx_t s = isqrt_u128(tmp);
    idx_t t = (s - 1) / 2;

    /* Compute exponent e = ((t*t + 3*t + 4)/2) - n. */
    idx_t e = ((t * t + 3 * t + 4) / 2) - n;
    if (e == 0)
        return (seq_t)1;                           /* n^0 = 1 */

    seq_t base = (seq_t)n;
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        result = result * base;                    /* exact for seq_t */

    return result;
}
