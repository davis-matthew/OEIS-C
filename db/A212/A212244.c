/*
 * A212244  Number of (w,x,y,z) with all terms in {1,...,n} and w + n = x*y*z.
 * https://oeis.org/A212244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (w,x,y,z) in {1,…,n} with w + n =
 *            x*y*z Compute by summing over all pairs (x,y) and counting
 *            admissible z
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static inline idx_t ceil_div128(idx_t a, __int128 b) {
    return (idx_t)(((__int128)a + b - 1) / b);
}
static inline idx_t floor_div128(idx_t a, __int128 b) {
    return (idx_t)( (__int128)a / b );
}

seq_t a(idx_t n) {
    seq_t result = 0;
    /* For n = 0 or 1 the answer is 0, the loops handle it automatically */
    __int128 two_n128 = (__int128)2 * n;
    for (idx_t x = 1; x <= n; ++x) {
        idx_t max_y = (idx_t)(two_n128 / x);
        if (max_y > n) max_y = n;
        for (idx_t y = 1; y <= max_y; ++y) {
            __int128 prod = (__int128)x * y;          /* product xy */
            /* Compute bounds for z:  n+1 <= x*y*z <= 2n  and 1 <= z <= n */
            idx_t low_z  = ceil_div128((idx_t)(n + 1), prod);
            if (low_z < 1) low_z = 1;
            idx_t high_z = floor_div128(two_n128, prod);
            if (high_z > n) high_z = n;
            if (low_z <= high_z) {
                result += (seq_t)(high_z - low_z + 1);
            }
        }
    }
    return result;
}
