/*
 * A081884  Number of steps needed to reach an integer starting with n+1/8 and
 *          iterating the map x-->x*ceiling(x).
 * https://oeis.org/A081884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 67, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081884:  a(n) = number of steps to reach an integer
   starting from n + 1/8 and iterating x -> x * ceil(x).  The
   algorithm keeps the fraction x = num/den (den = 8) and
   iterates until den divides num. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* den is always 8, numerator starts at 8*n + 1 */
    const seq_t den = (seq_t)8;
    seq_t num   = (seq_t)n * den + (seq_t)1;
    seq_t steps = 0;

    /* iterate until numerator is divisible by 8 */
    while (num % den != 0) {
        /* ceil(num/den) = (num + den - 1)/den */
        seq_t ceil_val = (num + den - 1) / den;
        num = num * ceil_val;
        steps++;
    }

    return steps;   /* steps is small (≤~30 for n≤10000) */
}
