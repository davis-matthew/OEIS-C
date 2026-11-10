/*
 * A204552  Permanent of the n-th principal submatrix of A204551.
 * https://oeis.org/A204552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n X n matrix M(i,j) = floor((i+j+2)/4),
 *            1 <= i,j <= n (the leading n X n block of A204551); computed by
 *            Ryser's formula over Gray-code subsets
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    int m = (int)n;
    long long r[64];
    bool on[64];
    for (int i = 0; i < m; i++) { r[i] = 0; on[i] = false; }
    seq_t pos = 0, neg = 0;
    unsigned long long total = 1ULL << m;
    int size = 0;
    for (unsigned long long k = 1; k < total; k++) {
        int c = 0;
        unsigned long long t = k;
        while (!(t & 1ULL)) { t >>= 1; c++; }
        long long d;
        if (on[c]) { on[c] = false; d = -1; size--; }
        else       { on[c] = true;  d = 1;  size++; }
        bool zero = false;
        for (int i = 0; i < m; i++) {
            r[i] += d * (long long)((i + c + 4) / 4);
            if (r[i] == 0) zero = true;
        }
        if (zero) continue;
        seq_t p = (seq_t)r[0];
        for (int i = 1; i < m; i++) p = p * (seq_t)r[i];
        if (((m - size) & 1) == 0) pos += p;
        else neg += p;
    }
    return pos - neg;
}
