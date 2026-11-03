/*
 * A075312  Products of Wythoff pairs: [n*r]*[n*r^2], where [] is the floor
 *          function and r is the golden ratio, (1+sqrt(5))/2.
 * https://oeis.org/A075312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi) * (floor(n*phi) + n), where phi =
 *            (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    seq_t n_seq   = (seq_t)n;
    seq_t n2      = n_seq * n_seq;
    seq_t five_n2 = n2 * (seq_t)5;
    seq_t sqrt5n  = floor_sqrt(five_n2);               /* floor(n*sqrt(5)) */
    seq_t a_val   = (n_seq + sqrt5n) / 2;              /* floor(n*phi)   */
    return a_val * (a_val + n_seq);                   /* product with n+a_val */
}
