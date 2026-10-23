/*
 * A122248  a(n) - a(n-1) = A113474(n).
 * https://oeis.org/A122248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+3)/2 - Sum_{k=1..n} popcount(k) (popcount = number
 *            of 1 bits in k)
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t sum_pop = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum_pop += (seq_t)__builtin_popcountll((uint64_t)k);
    }
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn + 3) / 2 - sum_pop;
    return val;
}
