/*
 * A090431  Difference between sums of digits of n and n-th prime.
 * https://oeis.org/A090431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = digit_sum(n) - digit_sum(p_n)
 * Range:     i8 n <= 31, i16 n <= 3566, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   digit_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_n is the n-th prime (p_1 = 2). */

#include "../utils/digit_sum.h"
#include "../utils/get_prime.h"

/*
 * Compute the difference between the sum of decimal digits of n
 * and the sum of decimal digits of the n-th prime.
 *
 * The function is compatible with the generic build environment
 * for seq_t and idx_t.  It simply casts the idx_t values to seq_t
 * before performing digit_sum, which works for any integer type
 * of seq_t.
 */
seq_t a(idx_t n)
{
    seq_t sum_n = digit_sum((seq_t)n);
    seq_t sum_p = digit_sum((seq_t)get_prime(n));
    return sum_n - sum_p;
}
