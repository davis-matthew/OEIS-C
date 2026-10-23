/*
 * A199536  The first column in Clark Kimberling's even first column Stolarsky
 *          array (beginning column count at 1).
 * https://oeis.org/A199536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 if n = 1,
 *              a(2k)   = 2*floor(k*phi) + 2k,
 *              a(2k+1) = a(2k) + 2,
 * where phi = (1+√5)/2.
 * The computation of floor(k*phi) uses only integer arithmetic.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128 (returns floor(sqrt(x))) */
static inline unsigned __int128 u128_sqrt(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = (unsigned __int128)1 << 65;  /* ceil sqrt(5*2^128) < 2^65 */

    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid <= x / mid)  /* avoid overflow: check mid*mid <= x */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* floor(k*phi) without floating point:
 *   floor(k*phi) = floor( (k + floor( sqrt(5*k*k) )) / 2 )
 */
static inline seq_t floor_phi_from_idx(idx_t k)
{
    unsigned __int128 k128 = (unsigned __int128)k;
    unsigned __int128 sq = k128 * k128 * (unsigned __int128)5;   /* 5*k^2 */
    unsigned __int128 sqrt_k = u128_sqrt(sq);                    /* floor(sqrt(5*k^2)) */
    unsigned __int128 num   = k128 + sqrt_k;                     /* k + floor(sqrt(5*k^2)) */
    unsigned __int128 half  = num >> 1;                          /* integer division by 2 */
    return (seq_t)half;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if ((n & 1U) == 0) {                  /* even n = 2k */
        idx_t k = n >> 1;
        seq_t fp = floor_phi_from_idx(k);
        return (seq_t)((unsigned __int128)2 * fp + (unsigned __int128)2 * k);
    } else {                               /* odd n = 2k+1 */
        idx_t k = n >> 1;                 /* floor((n-1)/2) */
        seq_t fp = floor_phi_from_idx(k);
        seq_t even = (seq_t)((unsigned __int128)2 * fp + (unsigned __int128)2 * k);
        return even + (seq_t)2;
    }
}
