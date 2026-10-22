/*
 * A002113  Palindromes in base 10.
 * https://oeis.org/A002113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice, core
 * Formula:   a(1) = 0; for n >= 2 let m = n-1. There are 9*10^(h-1)
 *            palindromes with d digits, where h = ceil(d/2). Take m-th
 *            positive palindrome: find d, set half = 10^(h-1) + (offset-1),
 *            then a(n) = half followed by reversed digits of half (dropping
 *            half's last digit when d is odd)
 * Range:     u8 n <= 35, u16 n <= 754, u32 n <= 9663, u64 n <= 9663,
 *            u128 n <= 9663, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    idx_t m = n - 1;
    unsigned d = 1;
    idx_t p = 1; /* 10^(h-1) */
    for (;;) {
        unsigned h = (d + 1) / 2;
        idx_t pw = 1;
        unsigned i;
        int big = 0;
        for (i = 1; i < h; i++) {
            if (pw > UINT64_MAX / 10) { big = 1; break; }
            pw *= 10;
        }
        if (big || pw > UINT64_MAX / 9) { p = pw; break; }
        idx_t cnt = 9 * pw;
        if (m <= cnt) { p = pw; break; }
        m -= cnt;
        d++;
    }
    idx_t half = p + (m - 1);
    seq_t res = (seq_t)half;
    idx_t t = (d % 2 == 1) ? half / 10 : half;
    while (t > 0) {
        res = res * 10 + (seq_t)(t % 10);
        t /= 10;
    }
    return res;
}
