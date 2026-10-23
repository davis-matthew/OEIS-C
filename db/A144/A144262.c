/*
 * A144262  a(n) = smallest k such that k*n is not a Niven (or Harshad)
 *          number.
 * https://oeis.org/A144262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that k*n is not a Niven number
 * Range:     u8 n <= 17, u16 n <= 539, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires: digit_sum from utils (../utils/digit_sum.h) */
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* n >= 1; return the smallest k > 0 with k*n not divisible by its digit sum */
    for (idx_t k = 1; ; ++k) {
        seq_t prod = (seq_t)k * (seq_t)n;       /* k*n */
        seq_t sum  = digit_sum(prod);          /* sum of decimal digits of prod */
        /* sum is always > 0 here (prod > 0) */
        if (prod % sum != 0)                   /* not Niven */
            return (seq_t)k;
    }
}
