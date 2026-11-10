/*
 * A072453  Shadow transform of A000522.
 * https://oeis.org/A072453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = number of k in 0..n-1 with A000522(k) == 0 (mod n), a(0)
 *            = 0; A000522(k) = k*A000522(k-1)+1, A000522(0) = 1 (shadow
 *            transform)
 * Range:     u8 n <= 1723, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9579 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t m = (seq_t)n;
    seq_t r = 1 % m;
    seq_t count = 0;
    if (r == 0) count++;
    for (idx_t k = 1; k < n; k++) {
        r = (((seq_t)k % m) * r + 1) % m;
        if (r == 0) count++;
    }
    return count;
}
