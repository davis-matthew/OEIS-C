/*
 * A066548  Sum of digits of n-th term of A066547.
 * https://oeis.org/A066548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit sum of A066547(n), i.e. the sum of the n digits of
 *            the Champernowne string 123456789101112... starting at digit
 *            n(n-1)/2+1 (leading zeros add 0, so the digits are summed
 *            directly)
 * Range:     u8 n <= 50, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    uint64_t p = (uint64_t)n * ((uint64_t)n - 1) / 2 + 1; /* 1-based digit position */
    uint64_t d = 1, count = 9, start = 1;
    while (p > d * count) {
        p -= d * count;
        d++;
        count *= 10;
        start *= 10;
    }
    uint64_t cur = start + (p - 1) / d;
    uint64_t off = (p - 1) % d;
    uint64_t taken = 0;
    uint64_t sum = 0;
    while (taken < (uint64_t)n) {
        char buf[24];
        int len = 0;
        uint64_t t = cur;
        while (t > 0) {
            buf[len++] = (char)(t % 10);
            t /= 10;
        }
        /* buf holds digits least-significant first; read from most significant */
        for (int i = len - 1 - (int)off; i >= 0 && taken < (uint64_t)n; --i) {
            sum += (uint64_t)buf[i];
            ++taken;
        }
        off = 0;
        ++cur;
    }
    return (seq_t)sum;
}
