/*
 * A143996  Rectangular array by antidiagonals: label each unit square in the
 *          first quadrant lattice by its northeast vertex (x,y) and mark
 *          squares for which (x,y) is congruent mod 4 to one of the
 *          following: (1,4), (2,2), (3,3), (4,1); then R(m,n) is the number
 *          of marked squares in the rectangle [0,m]x[0,n].
 * https://oeis.org/A143996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor( m(n) * k(n) / 4 ) where m(n) and k(n) are derived
 *            from n by antidiagonal enumeration: let s be the minimal integer
 *            with s*(s-1)/2 >= n then m = n - (s-2)*(s-1)/2, k = s - m
 * Range:     u8 n <= 933, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 128‑bit unsigned integer */
static inline idx_t
floor_sqrt_uint128(unsigned __int128 x)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 34;  /* > sqrt(2^67) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds, safeguard */

    /* find minimal s such that s*(s-1)/2 >= n */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;
    idx_t sqrt_val = floor_sqrt_uint128(t);
    idx_t s = (sqrt_val + 1) / 2;
    if ((unsigned __int128)s * (s - 1) / 2 < n)
        ++s;

    /* number of terms before diagonal s */
    idx_t terms_before = (idx_t)((unsigned __int128)(s - 2) * (s - 1) / 2);

    /* 0‑based offset within diagonal */
    idx_t offset = n - terms_before - 1;

    /* coordinates (m, k) */
    idx_t m = offset + 1;
    idx_t k = s - m;

    /* compute floor(m*k/4) */
    seq_t prod = (seq_t)m * (seq_t)k;
    return prod / 4;
}
