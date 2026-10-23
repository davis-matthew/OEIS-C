/*
 * A138471  Number of numbers less than n having the same sum of digits.
 * https://oeis.org/A138471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 3361, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A138471:  a(n) = #{ 0 ≤ k < n  |  digit_sum(k) = digit_sum(n) }   */
/* Calculation is performed by a straightforward scan up to n,
   which is fast enough for n ≤ 10 000 (the range needed for the
   evaluation).  The routine uses only integer arithmetic compatible
   with all supported seq_t / idx_t types. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: sum of decimal digits of a non‑negative integer.
   The argument is taken as a universal integer type (idx_t) which
   can safely hold the values up to the callers' limits. */
static seq_t digit_sum(idx_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

/* The requested OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset 0 → first term 0 */

    seq_t target = digit_sum(n);
    seq_t count = 0;

    /* Count numbers < n that have the same digit sum. */
    for (idx_t k = 0; k < n; ++k) {
        if (digit_sum(k) == target) ++count;
    }
    return count;
}
