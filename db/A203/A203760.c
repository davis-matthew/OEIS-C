/*
 * A203760  v(n+1)/v(n), where v=A203759.
 * https://oeis.org/A203760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (x_i^2 + x_i*m + m^2), where x_i =
 *            ceil(i/2) and m = ceil((n+1)/2); this equals v(n+1)/v(n) with v
 *            = A203759
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t m = ((seq_t)n + 2) / 2;      /* ceil((n+1)/2) */
    seq_t mm = m * m;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t x = ((seq_t)i + 1) / 2;  /* ceil(i/2) */
        result *= x * x + x * m + mm;
    }
    return result;
}
