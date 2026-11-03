/*
 * A071764  Number of minimal rectangular envelopes (up to rotation) that
 *          enclose n contiguous squares.
 * https://oeis.org/A071764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + ceil(n/2) - ceil(tau(n-1)/2) (a(0)=1)
 * Range:     u8 n <= 34, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-02)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (n(n+1)/2 + floor((n+1)/2) - isqrt(n-1) - D(n-1)) / 2 for n >= 1, where
   D(m) = Sum_{i=1..m} floor(m/i), the divisor summatory function (from the
   PARI program in the entry); D(m) = 2 Sum_{i<=s} floor(m/i) - s^2 with
   s = isqrt(m). Everything is computed exactly in 128 bits. */

#include <stdint.h>

static uint64_t isqrt_u64(uint64_t m)
{
    uint64_t r = 0;
    for (uint64_t bit = UINT64_C(1) << 62; bit; bit >>= 2) {
        if (m >= r + bit) { m -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;
    uint64_t m = (uint64_t)n - 1, s = isqrt_u64(m);
    unsigned __int128 d = 0;
    for (uint64_t i = 1; i <= s; ++i) d += m / i;
    d = 2 * d - (unsigned __int128)s * s;
    unsigned __int128 t = (unsigned __int128)n * (n + 1) / 2 + ((uint64_t)n + 1) / 2 - s - d;
    return (seq_t)(t / 2);
}
