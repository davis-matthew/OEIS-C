/*
 * A211996  Number of ordered pairs (i,j) such that i*j=n and i+j is a square.
 * https://oeis.org/A211996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (i,j) with i*j = n and i+j a
 *            perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for unsigned 64‑bit values, implemented without
   overflow.  Returns floor(sqrt(x)). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;          /* highest power of four ≤ x */
    while (bit > x) bit >>= 2;              /* move to valid starting bit */

    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j   = n / i;
            idx_t sum = i + j;          /* sum fits in idx_t (≤ 2*n) */
            idx_t sq  = isqrt_u64(sum);
            if (sq * sq == sum) {
                /* ordered pair: (i,j) and (j,i) when i≠j,
                   single pair when i==j (perfect square factor) */
                if (i == j) count += (seq_t)1;
                else        count += (seq_t)2;
            }
        }
    }
    return count;
}
