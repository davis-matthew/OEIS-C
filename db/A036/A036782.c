/*
 * A036782  a(n) = n - 1 + Sum_{j=0..n} j!.
 * https://oeis.org/A036782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 1 + Sum_{j=0..n} j!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t fact = 1;
    for (idx_t i = 0; i <= n; ++i) {
        if (i > 0) fact *= (seq_t)i;
        sum += fact;
    }
    return sum + (seq_t)n - 1;
}
