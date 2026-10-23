/*
 * A174805  n+ceiling[sqrt(n)]+floor[sqrt(n)].
 * https://oeis.org/A174805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(n)) + ceil(sqrt(n))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt(idx_t x)
{
    /* Compute floor(sqrt(x)) for 64‑bit unsigned integers */
    idx_t r = 0;
    idx_t bit = 1ULL << 62;          /* highest power of 4 <= 2^64 */

    while (bit > x) bit >>= 2;       /* find starting bit */

    for (; bit != 0; bit >>= 2) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t k   = floor_sqrt(n);                 /* floor(sqrt(n)) */
    seq_t kk  = (seq_t)k * (seq_t)k;           /* k²  */
    seq_t ceil = (seq_t)k;
    if (kk != (seq_t)n) ceil += 1;             /* ceil(sqrt(n)) */

    return (seq_t)n + (seq_t)k + ceil;
}
