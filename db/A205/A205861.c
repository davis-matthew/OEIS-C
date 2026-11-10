/*
 * A205861  Positions of multiples of 7 in A204922 (differences of Fibonacci
 *          numbers).
 * https://oeis.org/A205861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th multiple of 7 in A204922, where
 *            A204922(m) = F(r+2) - F(c+1) for row r and column c (1 <= c <=
 *            r), m = r(r-1)/2 + c. Computed by a search over rows using
 *            Fibonacci numbers mod 7 (period 16)
 * Range:     u8 n <= 35, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned fibmod7_A205861(idx_t k) {
    static const unsigned char t[16] = {0,1,1,2,3,5,1,6,0,6,6,5,4,2,6,1};
    return t[k & 15];
}

seq_t a(idx_t n) {
    idx_t counts[7] = {0,0,0,0,0,0,0};
    idx_t remaining = n;
    for (idx_t row = 1; ; ++row) {
        counts[fibmod7_A205861(row + 1)]++;
        unsigned target = fibmod7_A205861(row + 2);
        idx_t c = counts[target];
        if (remaining > c) {
            remaining -= c;
            continue;
        }
        idx_t seen = 0;
        for (idx_t col = 1; col <= row; ++col) {
            if (fibmod7_A205861(col + 1) == target) {
                ++seen;
                if (seen == remaining)
                    return (seq_t)(row * (row - 1) / 2 + col);
            }
        }
    }
}
