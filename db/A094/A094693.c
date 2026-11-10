/*
 * A094693  Records in A094685.
 * https://oeis.org/A094693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>=1, a(n) = round((2n-1)^(3/2)), since the records
 *            in A094685 are its values at odd arguments
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A094693_isqrt(seq_t m)
{
    seq_t b = 1, r = 0;
    while (b <= m / b)
        b <<= 1;
    for (b >>= 1; b > 0; b >>= 1) {
        seq_t c = r + b;
        if (c <= m / c)
            r = c;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t t = (seq_t)2 * (seq_t)n - 1;
    seq_t m = t * t * t;
    seq_t k = A094693_isqrt(m);
    /* round(sqrt(m)) = k+1 iff m > k^2 + k */
    if (m - k * k > k)
        return k + 1;
    return k;
}
