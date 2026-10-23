/*
 * A126452  Column 1 of triangle A126450; a(n) = C( C(n+3,3), n).
 * https://oeis.org/A126452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C( C(n+3,3), n )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = C( C(n+3,3), n ) */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* n as seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* m = C(n+3,3) = (n+3)*(n+2)*(n+1)/6 */
    seq_t m = (sn + 3) * (sn + 2) * (sn + 1) / 6;

    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* term = m - n + i */
        seq_t term = m - sn + (seq_t)i;
        /* res = res * term / i */
        res = (res * term) / (seq_t)i;
    }

    return res;
}
