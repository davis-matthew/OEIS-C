/*
 * A190332  n + [n*s/r] + [n*t/r]; r=1, s=sqrt(3), t=1/s.
 * https://oeis.org/A190332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n*sqrt(3)] + [n/sqrt(3)]
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only. */

#include <stdint.h>

/* Compute floor(sqrt(x)) for 128‑bit unsigned integer x.
   The result fits in seq_t as the ceiling of the required
   values for the sequence. */
static inline seq_t uint128_sqrt(unsigned __int128 x) {
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64; /* hi = 2^64 */
    while (lo + 1 < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        /* test if mid*mid <= x without overflow:
           mid*mid > x  <=>  x / mid < mid  (mid > 0)            */
        if (mid != 0 && x / mid < mid) {
            hi = mid;      /* mid too big */
        } else {
            lo = mid;      /* mid possible */
        }
    }
    return (seq_t)lo;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1 but harmless */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 prod = nn * nn * 3;   /* 3*n^2 */
    seq_t sqrt_val = uint128_sqrt(prod);   /* floor(n*sqrt(3)) */
    seq_t result = (seq_t)n + sqrt_val + sqrt_val / 3;
    return result;
}
