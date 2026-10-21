/*
 * A069250  Sum of the reversals of the proper divisors of n.
 * https://oeis.org/A069250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 9632, u32 n <= 9632, u64 n <= 9632,
 *            u128 n <= 9632, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = sum of the reversals of the proper divisors of n.
   Formula: a(n) = Σ_{d|n, d<n} reverse(d) */
#include <stdint.h>    /* for idx_t (uint64_t) */

static seq_t rev_uint64(idx_t x)
{
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t sum = 0;

    /* compute floor(sqrt(n)) without overflow */
    idx_t limit = 0;
    while ((limit + 1) <= n / (limit + 1)) {
        limit++;
    }

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            /* d is always a proper divisor for n > 1 */
            sum += rev_uint64(d);

            idx_t d2 = n / d;
            if (d2 != d && d2 != n) {
                sum += rev_uint64(d2);
            }
        }
    }
    return sum;
}
