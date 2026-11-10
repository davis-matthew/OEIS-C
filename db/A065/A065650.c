/*
 * A065650  Inverse permutation to A065649.
 * https://oeis.org/A065650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = position (1-based) in Champernowne's digit string
 *            123456789101112... of the (floor(m/10)+1)-th occurrence of digit
 *            d=m mod 10 (the h-th if d=0, the leading 0 counting as the
 *            first), with m=n; a(0)=0. Computed by scanning
 * Range:     u8 n <= 222, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9653 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t mm = (seq_t)n;
    int d = (int)(mm % 10);
    uint64_t h = (uint64_t)(long long)(mm / 10);
    uint64_t k = (d == 0) ? h : h + 1;   /* which occurrence to find */

    uint64_t cnt = 0;
    uint64_t pos = 0;
    for (uint64_t num = 1;; ++num) {
        char buf[24];
        int len = 0;
        uint64_t t = num;
        while (t > 0) {
            buf[len++] = (char)(t % 10);
            t /= 10;
        }
        for (int i = len - 1; i >= 0; --i) {
            ++pos;
            if (buf[i] == d) {
                ++cnt;
                if (cnt == k)
                    return (seq_t)pos;
            }
        }
    }
}
