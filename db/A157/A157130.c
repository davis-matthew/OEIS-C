/*
 * A157130  Partial sums of A128201.
 * https://oeis.org/A157130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - r)^2 + (4*r^3 + 6*r^2 + 2*r)/3 * where r =
 *            floor((sqrt(1 + 8*n) - 1)/4)
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned __int128, result fits in uint64_t */
static uint64_t floor_sqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64); /* 2^64 */

    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;

        if (sq == x)
            return (uint64_t)mid;

        if (sq < x) {
            low = mid + 1;
        } else {
            /* mid>0 because x>=y*y for some y>=1 */
            high = mid - 1;
        }
    }
    /* low is first value with sq > x, so (low-1) is floor sqrt */
    return (uint64_t)(low - 1);
}

seq_t a(idx_t n)
{
    /* safety: offset starts at 1 but we just compute formula for n>=1 */
    unsigned __int128 tmp = (unsigned __int128)1 + (unsigned __int128)8 * n;
    uint64_t s   = floor_sqrt_u128(tmp);
    uint64_t r   = (s >= 1) ? (s - 1) / 4 : 0;

    seq_t n_sq   = (seq_t)n;
    seq_t r_sq   = (seq_t)r;

    /* (n - r)^2 */
    seq_t diff   = n_sq - r_sq;
    seq_t part1  = diff * diff;

    /* (4*r^3 + 6*r^2 + 2*r) / 3 */
    seq_t r2 = r_sq * r_sq;
    seq_t r3 = r2 * r_sq;
    seq_t part3 = ((seq_t)4 * r3 + (seq_t)6 * r2 + (seq_t)2 * r_sq) / (seq_t)3;

    return part1 + part3;
}
