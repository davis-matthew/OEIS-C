/*
 * A092599  Number of those natural numbers x for which sum of decimal digits
 *          exceeds x/n.
 * https://oeis.org/A092599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of natural numbers x for which
 *            sum_of_decimal_digits(x) > x / n
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function scans all candidates up to the largest possible x that can satisfy the
   inequality by estimating the maximal number of digits that can appear.              */

#include <stdint.h>
#include <stdbool.h>

/* Computes the sum of decimal digits of a 64‑bit unsigned integer.
   The result never exceeds 9 * number_of_digits, i.e. < 170.              */
static inline seq_t digit_sum_u64(uint64_t v)
{
    seq_t sum = 0;
    while (v) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Returns 10^k as a seq_t.      */
static inline seq_t pow10_seq_t(unsigned k)
{
    seq_t p = 1;
    while (k--) p *= 10;
    return p;
}

seq_t a(idx_t n)
{
    /* For n=0 the list is undefined, but we return 0 for safety.            */
    if (n == 0) return 0;

    /* Find largest digit length d such that n * 9 * d >= 10^(d-1).          */
    unsigned d = 1;                     /* current digit count            */
    while (true) {
        seq_t upper = (seq_t)n * 9 * d;
        seq_t thresh = pow10_seq_t(d - 1);     /* 10^(d-1)                   */
        if (upper < thresh) break;
        ++d;
    }
    unsigned max_d = d - 1;                 /* last satisfying digit count   */

    seq_t bound  = (seq_t)n * 9 * max_d;    /* n * 9 * max_d                 */
    seq_t pow10  = pow10_seq_t(max_d);
    seq_t max10  = pow10 * 10 - 1;          /* 10^max_d - 1                  */
    seq_t upper  = (bound < max10) ? bound : max10;

    seq_t count = 0;

    /* Linear scan up to the upper bound.                                   */
    for (uint64_t x = 1; x <= (uint64_t)upper; ++x) {
        seq_t n_times_sum = (seq_t)n * digit_sum_u64(x);
        if (n_times_sum > (seq_t)x) ++count;
    }
    return count;
}
