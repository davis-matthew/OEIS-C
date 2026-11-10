/*
 * A050810  From sequence A050782, the n's corresponding to the first
 *          occurrence of m (or 0 if not defined).
 * https://oeis.org/A050810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = least k such that A050782(k) = n (k*n is then the
 *            smallest palindromic multiple of k); 0 if none (n=0 or n
 *            multiple of 10). Computed by search over palindromes P divisible
 *            by n, in increasing order
 * Range:     u8 n <= 4, u16 n <= 52, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A050810_is_pal(seq_t x)
{
    uint8_t d[160];
    int len = 0;
    while (x > (seq_t)0) {
        d[len++] = (uint8_t)(x % (seq_t)10);
        x /= (seq_t)10;
    }
    for (int i = 0, j = len - 1; i < j; ++i, --j)
        if (d[i] != d[j]) return false;
    return true;
}

static seq_t A050810_build(seq_t half, int len)
{
    seq_t pal = half;
    seq_t t = (len % 2) ? half / (seq_t)10 : half;
    while (t > (seq_t)0) {
        pal = pal * (seq_t)10 + t % (seq_t)10;
        t /= (seq_t)10;
    }
    return pal;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    if (n % (idx_t)10 == (idx_t)0) return (seq_t)0;
    seq_t m = (seq_t)n;

    seq_t lower = (seq_t)1;
    for (int len = 1; ; ++len) {
        int k = (len + 1) / 2;
        if (len > 1 && (len % 2) == 1) lower *= (seq_t)10;
        else if (len == 1) lower = (seq_t)1;
        seq_t upper = lower * (seq_t)10 - (seq_t)1;
        for (seq_t h = lower; h <= upper; ++h) {
            seq_t pal = A050810_build(h, len);
            if (pal % m != (seq_t)0) continue;
            seq_t c = pal / m;
            bool ok = true;
            for (seq_t j = (seq_t)1; j < m; ++j) {
                if (A050810_is_pal(j * c)) { ok = false; break; }
            }
            if (ok) return c;
        }
        (void)k;
    }
}
