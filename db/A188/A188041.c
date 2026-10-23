/*
 * A188041  a(n) = [n*r]-[k*r]-[n*r-k*r], where r=sqrt(2), k=3, [ ]=floor.
 * https://oeis.org/A188041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(2)) - floor(3*sqrt(2)) -
 *            floor((n-3)*sqrt(2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with sqrt(2) the square root of 2, floor(3*sqrt(2)) = 4. */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for unsigned __int128, returns the floor of the root. */
static inline seq_t floor_sqrt_u128(unsigned __int128 t) {
    unsigned __int128 low = 0;
    unsigned __int128 high = (unsigned __int128)1 << 64;  /* sqrt(t) <= 2^64-1 */
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= t)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;  /* fits in seq_t for n <= 10000 */
}

/* floor(k * sqrt(2)) for signed k. */
/* Uses the identity floor(k*sqrt(2)) = floor(sqrt(2*k^2)) for k >= 0,  */
/* and floor(-m*sqrt(2)) = -(floor(m*sqrt(2)) + 1) for m > 0. */
static inline int64_t floor_k_sqrt2_signed(int64_t k) {
    if (k >= 0) {
        unsigned __int128 t = (unsigned __int128)2 *
                             (unsigned __int128)k *
                             (unsigned __int128)k;
        return (int64_t)floor_sqrt_u128(t);
    } else {
        int64_t m = -k;
        unsigned __int128 t = (unsigned __int128)2 *
                             (unsigned __int128)m *
                             (unsigned __int128)m;
        int64_t floor_m = (int64_t)floor_sqrt_u128(t);
        return -(floor_m + 1);   /* ceil(m*sqrt(2)) = floor_m + 1 */
    }
}

seq_t a(idx_t n) {
    int64_t fn  = floor_k_sqrt2_signed((int64_t)n);
    int64_t fn3 = floor_k_sqrt2_signed((int64_t)n - 3);
    int64_t res = fn - 4 - fn3;  /* floor(3*sqrt(2)) = 4 */
    return (seq_t)res;
}
