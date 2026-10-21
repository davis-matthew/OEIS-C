/*
 * A080381  Triangle read by rows: gcd(binomial(n,floor(n/2)), binomial(n,i)),
 *          i=0..n; greatest common divisor of binomial coefficients and
 *          corresponding central binomial coefficient.
 * https://oeis.org/A080381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = gcd( binomial(r, floor(r/2)), binomial(r, i) ), where r =
 *            floor((sqrt(8*n+1)-1)/2) and i = n - r*(r+1)/2
 * Range:     u8 n <= 37, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/binomial.h"

/* Determine the row (r) and column (i) from the flattened index n,
   then return the gcd of C(r, floor(r/2)) and C(r, i). */
seq_t a(idx_t n)
{
    /* binary search for r: smallest r such that T_r = r*(r+1)/2 > n */
    idx_t lo = 0;
    idx_t hi = (n <= 1) ? 2 : n;   /* hi always satisfies T_hi > n for n>=0 */

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        /* use unsigned 128‑bit to avoid overflow: 0 <= mid <= hi <= 2^64-1 */
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid + 1;
        else
            hi = mid;
    }

    idx_t r = lo - 1;
    unsigned __int128 prefix = (unsigned __int128)r * (r + 1) / 2;
    idx_t i = n - (idx_t)prefix;

    /* central binomial coefficient C(r, floor(r/2)) */
    idx_t k_center = r >> 1;              /* floor(r/2) */
    seq_t central = binomial(r, k_center);

    /* binomial coefficient C(r, i) */
    seq_t val = binomial(r, i);

    return gcd(central, val);
}
