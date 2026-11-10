/*
 * A094608  Rectangular array T by antidiagonals: row n consists of ranks of n
 *          in A000119.
 * https://oeis.org/A094608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(i,j) = 1 + (j-th m >= 0 with A000119(m) = i); a(n) reads T by
 *            antidiagonals, T(i,j) with i = k, j = d+1-k for n = d(d-1)/2 +
 *            k, 1<=k<=d. Computed by a Fibonacci subset-sum DP with doubling
 *            limit
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    uint64_t d = 1;
    uint64_t row, col;
    uint64_t L = 256;
    while ((uint64_t)d * (d + 1) / 2 < (uint64_t)n) d++;
    row = (uint64_t)n - d * (d - 1) / 2;
    col = d + 1 - row;
    for (;;) {
        uint32_t *cnt = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
        uint64_t f1 = 1, f2 = 2, m, found = 0;
        if (!cnt) return 0;
        cnt[0] = 1;
        while (f1 < L) {
            for (m = L - 1; m >= f1; m--) cnt[m] += cnt[m - f1];
            {
                uint64_t t = f1 + f2;
                f1 = f2;
                f2 = t;
            }
        }
        for (m = 0; m < L; m++) {
            if ((uint64_t)cnt[m] == row) {
                found++;
                if (found == col) {
                    free(cnt);
                    return (seq_t)(m + 1);
                }
            }
        }
        free(cnt);
        if (L >= ((uint64_t)1 << 30)) return 0;
        L *= 2;
    }
}
