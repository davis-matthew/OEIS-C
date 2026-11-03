/*
 * A000141  Number of ways of writing n as a sum of 6 squares.
 * https://oeis.org/A000141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*(Sum_{d|n, d==3 mod 4} d^2 - Sum_{d|n, d==1 mod 4} d^2)
 *            + 16*(Sum_{d|n, n/d==1 mod 4} d^2 - Sum_{d|n, n/d==3 mod 4}
 *            d^2), a(0) = 1 [Jacobi]
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9775 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void a141_add_div(idx_t n, idx_t d, seq_t *pos, seq_t *neg)
{
    seq_t sq = (seq_t)d * (seq_t)d;
    idx_t q = n / d;
    if (d % 4 == 3) *pos += 4 * sq;
    else if (d % 4 == 1) *neg += 4 * sq;
    if (q % 4 == 1) *pos += 16 * sq;
    else if (q % 4 == 3) *neg += 16 * sq;
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t d;
    if (n == 0) return (seq_t)1;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            a141_add_div(n, d, &pos, &neg);
            if (d != n / d) a141_add_div(n, n / d, &pos, &neg);
        }
    }
    return pos - neg;
}
