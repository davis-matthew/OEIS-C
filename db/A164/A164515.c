/*
 * A164515  Positive numbers not of the form n^2+2.
 * https://oeis.org/A164515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 + floor(sqrt(n-1) + 1/2)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>            /* for bool, but not strictly needed */

/* Integer square‑root: floor(sqrt(x)) for unsigned 64‑bit indices.
   Uses a fast bitwise algorithm and never overflows. */
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t result = 0;
    idx_t bit = 1ULL << 62;        /* largest power of two <= 2^63 */

    while (bit != 0) {
        if (result + bit <= x) {
            x -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    idx_t m = n - 1;
    idx_t s = _isqrt_u64(m);                 /* floor(sqrt(m)) */
    idx_t diff = m - s * s;                  /* m - s^2  (>=0)   */
    idx_t round_off = (diff >= s + 1) ? (s + 1) : s;  /* round(sqrt(m)) */

    return (seq_t)n + (seq_t)1 + (seq_t)round_off;
}
