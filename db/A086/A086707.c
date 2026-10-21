/*
 * A086707  Smallest mode of the sequences (n/(n-k)) * binomial(n,n-k).
 * https://oeis.org/A086707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(1/10*(5*n-4-sqrt(5*n*n-4)))
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for 64‑bit unsigned integers.
   Returns ⌊√n⌋. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* highest power of four <= n */
    while (bit > n) bit >>= 2;
    for (; bit; bit >>= 2) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* S = 5*n*n - 4   */
    idx_t S = (idx_t)5 * n * n - 4;
    idx_t s = isqrt_u64(S);                 /* floor(sqrt(S)) */

    /* tmp = 5*n - 4 - s  (always non‑negative) */
    seq_t tmp = (seq_t)((idx_t)5 * n - 4) - (seq_t)s;

    /* a(n) = ceil(tmp / 10) = (tmp + 9) / 10 */
    return (tmp + 9) / 10;
}
