/*
 * A111853  Number of numbers m <= n such that 3 equals the first digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ m ≤ n : first decimal digit after decimal point of √m
 *            is 3 }
 * Range:     u8 n <= 10, u16 n <= 692, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function counts integers m ≤ n such that the fractional part of √m lies in
   the interval [0.3, 0.4).  The calculation is done exactly using integer
   arithmetic with the helper int_sqrt from utils. */

#include "../utils/int_sqrt.h"

seq_t a(idx_t n) {
    /* The result never exceeds n, so seq_t is ample for any needed type. */
    seq_t count = 0;

    /* floor(sqrt(n)) gives upper bound for integer part of sqrt(m). */
    idx_t maxp = int_sqrt(n);

    for (idx_t p = 0; p <= maxp; ++p) {
        /* Compute bounds for m such that p ≤ √m < p+1 and
           first decimal digit of √m equals 3.                     */
        seq_t p_sq = (seq_t)p * p;                     /* p² */
        seq_t low_num = 100 * p_sq + 60 * p + 9;        /* 100*(p² + 0.6p + 0.09) */
        seq_t high_num = 100 * p_sq + 80 * p + 16;      /* 100*(p² + 0.8p + 0.16) */

        seq_t low  = (low_num + 99) / 100;             /* ceil(low_num/100) */
        seq_t high = (high_num - 1) / 100;             /* floor((high_num-1)/100) */

        /* Ensure the range lies within [p², n] */
        if (low < p_sq) low = p_sq;
        if (high > n)   high = n;

        if (high >= low) {
            count += high - low + 1;
        }
    }

    return count;
}
