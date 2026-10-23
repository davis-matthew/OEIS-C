/*
 * A176314  Sum of remainders of mod(n, k), for 1 <= k <= sqrt(n).
 * https://oeis.org/A176314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{floor(sqrt(n))} (n mod k)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values.
 * Returns the largest m such that m*m <= n. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;            /* highest power of four <= n */

    while (bit > n) bit >>= 2;

    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                    /* sequence starts at n=1 */

    idx_t limit = isqrt_u64(n);
    seq_t sum = 0;

    for (idx_t k = 1; k <= limit; ++k) {
        sum += (seq_t)(n % k);
    }
    return sum;
}
