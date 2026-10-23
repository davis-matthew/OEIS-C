/*
 * A153330  Differences in adjacent elements of the sequence quantifying the
 *          steps needed for n to converge to 1 in the Collatz Conjecture.
 * https://oeis.org/A153330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = A006577(n+1) - A006577(n)
 * Range:     i8 n <= 702, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   total_stopping_time
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A153330: difference of total stopping times for consecutive numbers */

#include <stdint.h>
#include "../utils/total_stopping_time.h"

/* public API: a(n) for n >= 1 */
seq_t a(idx_t n)
{
    idx_t t_n   = total_stopping_time(n);
    idx_t t_np1 = total_stopping_time(n + 1);
    return (seq_t)((long long)t_np1 - (long long)t_n);
}
