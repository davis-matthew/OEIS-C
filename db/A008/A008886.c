/*
 * A008886  Aliquot sequence starting at 42.
 * https://oeis.org/A008886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A008886: aliquot sequence starting from 42.
 *
 *  a(0) = 42
 *  a(n+1) = sum of proper divisors of a(n)   (Aliquot part)
 *  When a(n) becomes 0 all following terms are 0.
 *
 *  The implementation computes a(n) iteratively; no dynamic memory is
 *  required, so it compiles for both the unsigned integer builds and
 *  the arbitrary‑precision integer build. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t) */

/* Return the sum of proper divisors of a positive integer n.  For
 * n == 0 or 1 the result is 0, which matches the definition of the
 * aliquot part. */
static seq_t sum_proper_divisors(seq_t n)
{
    if (n <= 1)          /* 0 and 1 have no proper divisors */
        return 0;

    seq_t sqrt_n = floor_sqrt(n);
    seq_t sum = 0;

    for (seq_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {                 /* d is a divisor */
            seq_t other = n / d;

            if (d != n)                   /* add d if it is proper */
                sum += d;

            if (other != d && other != n) /* add complementary divisor if distinct */
                sum += other;
        }
    }

    return sum;
}

/* Aliquot sequence starting at 42.  Works for any n >= 0.
 * Subsequent calls are independent; no caching is necessary because
 * the sequence reaches 0 after a short number of steps. */
seq_t a(idx_t n)
{
    seq_t val = 42;               /* a(0) */

    if (n == 0)
        return val;

    for (idx_t i = 1; i <= n; ++i) {
        if (val == 0)          /* sequence has terminated */
            return 0;
        val = sum_proper_divisors(val);
    }
    return val;
}
