/*
 * A134319  Triangle read by rows. T(n, k) = binomial(n, k)*(2^k - 1 + 0^k).
 * https://oeis.org/A134319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) = 1 if k = 0 binomial(r,k) * (2^k - 1) if k > 0
 *            where r = floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2 (i.e.
 *            a(n) is the triangle read by rows with T(n,k) =
 *            binomial(n,k)*(2^k-1) for k>0, T(n,0)=1)
 * Range:     u8 n <= 31, u16 n <= 84, u32 n <= 266, u64 n <= 970,
 *            u128 n <= 3539, big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

/* Convert index n to row r and column k in the triangle */
static idx_t triangle_coords(idx_t n)
{
    /* Find the maximal r such that r*(r+1)/2 <= n using binary search. */
    idx_t low = 0, high = 1;
    while ( ((__int128)high * (high + 1)) / 2 <= (__int128)n ) {
        high *= 2;
    }
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ( ((__int128)mid * (mid + 1)) / 2 <= (__int128)n ) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t r = low;
    /* previous cumulative count */
    idx_t prev = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k     = n - prev;
    /* store k back */
    *((idx_t *)n) = k;  /* abusing pointer to return k via same n pointer? */
    return r;
}

seq_t a(idx_t n)
{
    /* find r and k */
    idx_t low = 0, high = 1;
    while ( ((__int128)high * (high + 1)) / 2 <= (__int128)n ) {
        high *= 2;
    }
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ( ((__int128)mid * (mid + 1)) / 2 <= (__int128)n ) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t r = low;
    idx_t prev = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - prev;

    if (k == 0) {
        return (seq_t)1;
    }

    seq_t bin = binomial(r, k);
    seq_t powk = pow_uint((uint64_t)2, k);      /* 2^k   */
    seq_t val  = bin * (powk - (seq_t)1);       /* (2^k-1) */
    return val;
}
