/*
 * A203584  v(n+1)/v(n), where v=A203583.
 * https://oeis.org/A203584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=2..n+1} (i^2 - i*(n+2) + (n+2)^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t j = (seq_t)n + 2;
    seq_t prod = 1;
    for (seq_t i = 2; i <= (seq_t)n + 1; i++) {
        seq_t term = i * i + j * j - i * j;
        prod *= term;
    }
    return prod;
}
