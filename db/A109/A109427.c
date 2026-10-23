/*
 * A109427  Numbers n such that sigma(n)/omega(n) is an integer [sigma(n) =
 *          sum of divisors of n; omega(n) = number of distinct prime factors
 *          of n].
 * https://oeis.org/A109427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   count_distinct_prime_factors, sum_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A109427 – numbers n for which sigma(n)/omega(n) is an integer. 
   The function iterates over candidates n = 2,3,… and counts those
   satisfying the condition, until the nth term is found. */
#include <stdbool.h>
#include "../utils/sum_divisors.h"
#include "../utils/count_distinct_prime_factors.h"

seq_t a(idx_t n)
{
    idx_t found = 0;          /* number of valid terms seen so far */
    for (idx_t num = 2; ; ++num) {
        seq_t sigma = sum_divisors(num);                 /* sigma(n) */
        idx_t omega = count_distinct_prime_factors(num); /* omega(n) */
        if (omega != 0 && sigma % (seq_t)omega == 0) {    /* condition holds */
            ++found;
            if (found == n)
                return (seq_t)num;                      /* return nth term */
        }
    }
}
