/*
 * A138108  Triangle read by rows: T(n,k) = n! * [t^n*x^k] x^(n+1) * exp(x*t)
 *          / (x-t), 0 <= k <= 2*n.
 * https://oeis.org/A138108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, tabf, changed
 * Formula:   let r = ⌊√n⌋, offset = n - r². If offset is odd, a(n) = 0. If
 *            offset is even, let m = offset/2. Then a(n) = r! / m! (the
 *            product (m+1)*(m+2)*…*r)
 * Range:     u8 n <= 35, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_idx(idx_t n) {
    /* Integer square root by Newton's method, works for 0..UINT64_MAX. */
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    idx_t r = isqrt_idx(n);
    idx_t offset = n - r * r;
    if (offset & 1)
        return (seq_t)0;
    idx_t m = offset >> 1;
    seq_t result = (seq_t)1;
    for (idx_t i = m + 1; i <= r; ++i)
        result *= (seq_t)i;
    return result;
}
