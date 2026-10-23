/*
 * A157279  Product 1*2*...*r mod n, where r = integer part of sqrt(n).
 * https://oeis.org/A157279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(n))! mod n
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for idx_t in case it's defined here */

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, x = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            x = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {          /* mid*mid <= n */
            x = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;               /* safety, though n≥1 by definition */
    }

    idx_t r = isqrt_u64(n);             /* floor(sqrt(n)) */
    seq_t mod = (seq_t)n;

    seq_t res = 1 % mod;                /* start factorial at 1 mod n */
    for (idx_t i = 1; i <= r; ++i) {
        res = (res * (seq_t)i) % mod;   /* multiply and reduce modulo n */
        if (res == 0) break;            /* early exit when factor is multiple of n */
    }
    return res;
}
