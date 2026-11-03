/*
 * A008885  Aliquot sequence starting at 30.
 * https://oeis.org/A008885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: full, pass, 16 terms through n = 15 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 30; a(n+1) = sigma(a(n)) – a(n) (proper divisor sum) */
#include <stdint.h>

/* Compute the sum of proper divisors of a positive integer x and
   return it as seq_t. Assumes x fits in uint64_t. */
static seq_t aliquot_part(uint64_t x)
{
    if (x <= 1)
        return 0;
    seq_t sum = 1;                     /* 1 is always a proper divisor for x > 1 */
    for (uint64_t d = 2; d * d <= x; ++d) {
        if (x % d == 0) {
            sum += (seq_t)d;
            uint64_t other = x / d;
            if (other != d)
                sum += (seq_t)other;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 30;
    seq_t cur = 30;
    for (idx_t i = 1; i <= n; ++i) {
        if (cur == 0)
            return 0;
        /* The current value fits in uint64_t – all intermediate sums
           also fit in seq_t (which is at least 64 bits). */
        cur = aliquot_part((uint64_t)cur);
    }
    return cur;
}
