/*
 * A220272  Number of ways to write n=x^2+y (x>0, y>0) with 2*x*y-1 prime.
 * https://oeis.org/A220272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs of positive integers (x, y) such that n =
 *            x^2 + y and 2*x*y - 1 is prime
 * Range:     u8 n <= 49, u16 n <= 2944, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 116 terms through n = 9661 (2026-11-01)
 * Helpers:   is_prime_seq, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t count = 0;
    idx_t maxx = isqrt(n - 1);        /* largest x with x^2 < n   */
    for (idx_t x = 1; x <= maxx; ++x) {
        idx_t y = n - x * x;                     /* y > 0 automatically   */
        seq_t p = (seq_t)2 * (seq_t)x * (seq_t)y - 1;
        if (is_prime_seq(p)) ++count;
    }
    return count;
}
