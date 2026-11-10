/*
 * A069829  a(n) = PS(n)(2n), where PS is described in A057032.
 * https://oeis.org/A069829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = PS(n)(2n), PS as in A057032; computed by tracing position
 *            2n back through the inverses of P(n), P(n-1), ..., P(2) applied
 *            to 1,2,3,
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9412 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t p = 2 * n;
    for (idx_t k = n; k >= 2; --k) {
        if (p >= k) {
            idx_t r = (p - k) % k;
            if (r > 0) p = p - 1;
            else p = p + k - 1;
        }
    }
    return (seq_t)p;
}
