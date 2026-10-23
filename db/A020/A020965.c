/*
 * A020965  a(n) = Sum_{k >= 1} floor(n*sqrt(2)^(1-k)).
 * https://oeis.org/A020965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k>=1} floor( n * sqrt(2)^(1-k) )
 * Range:     u8 n <= 11, u16 n <= 132, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Compute a(n) – seq_t is an unsigned integer type (possibly arbitrary precision) */
seq_t a(idx_t n)
{
    seq_t seq_n = (seq_t)n;
    seq_t even = seq_n;                     /* floor(n / 2^j)  (j = 0 initially) */
    /* odd_start = floor(n / sqrt(2))  computed once */
    seq_t tmp   = seq_n * seq_n;            /* n^2          */
    seq_t half  = tmp / 2;                  /* floor(n^2 / 2) */
    seq_t odd   = floor_sqrt(half);         /* floor( n / sqrt(2) ) */

    seq_t sum = 0;
    while (even > 0 || odd > 0) {
        sum += even;
        if (odd > 0) sum += odd;
        even = even / 2;                    /* next even term: floor(n / 2^{j+1})  */
        odd  = odd  / 2;                    /* next odd term  : floor(n / (2^{j+1} sqrt(2))) */
    }
    return sum;
}
