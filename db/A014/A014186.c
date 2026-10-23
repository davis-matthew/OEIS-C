/*
 * A014186  Squares of palindromes.
 * https://oeis.org/A014186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A002113(n)^2
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 754, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    idx_t idx = n - 1;          /* zero‑based index among all palindromes */
    idx_t L = 1;
    while (1) {
        idx_t count;
        if (L == 1)
            count = 10;        /* 0‑9 inclusive */
        else {
            idx_t half = (L + 1) / 2;
            idx_t pow = 1;
            for (idx_t i = 0; i < half - 1; ++i) pow *= 10;
            count = 9 * pow;
        }
        if (idx < count) break;
        idx -= count;
        ++L;
    }

    seq_t pal = 0;
    if (L == 1) {
        pal = (seq_t)idx;        /* 1‑9, 0 already handled */
        return pal * pal;
    }

    idx_t half = (L + 1) / 2;
    seq_t prefixLow = 1;
    for (idx_t i = 1; i < half; ++i) prefixLow *= 10;
    seq_t prefix = prefixLow + (seq_t)idx;

    pal = prefix;
    seq_t temp = prefix;
    if (L % 2 == 1) temp /= 10;
    while (temp > 0) {
        pal = pal * 10 + (temp % 10);
        temp /= 10;
    }

    return pal * pal;
}
