/*
 * A153274  Triangle, read by rows, T(n,k) = k^(n+1) * Pochhammer(1/k, n+1).
 * https://oeis.org/A153274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value in the triangle read by rows, where T(r,c) =
 *            Product_{j=0..r} (j*c + 1), and (r,c) are the row and column of
 *            the n-th element (rows and columns start at 1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 73, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) for values up to 2^65, using 128-bit arithmetic. */
static inline idx_t isqrt_u128(__int128 x)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 33;          /* > sqrt(2^65) < 2^33.5 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Find row r: the smallest r such that r*(r+1)/2 >= n. */
    __int128 val = (__int128)n * 8 + 1;   /* 1 + 8*n */
    idx_t t = isqrt_u128(val);           /* floor sqrt */
    if ((__int128)t * t < val)
        t++;                            /* ceil sqrt */
    idx_t r = t / 2;                     /* ceil((-1 + ceil_sqrt)/2) */

    /* Column c. */
    idx_t prevTri = (r - 1) * r / 2;
    idx_t c = n - prevTri;

    /* Compute product_{j=0..r} (j*c + 1). */
    seq_t result = 1;
    for (idx_t j = 0; j <= r; ++j) {
        seq_t factor = (seq_t)j * c + 1;
        result *= factor;
    }

    return result;
}
