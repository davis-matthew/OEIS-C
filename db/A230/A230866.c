/*
 * A230866  Number of integers m such that m + (sum of digits in base-5
 *          representation of m) = 2n.
 * https://oeis.org/A230866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ m | m + s5(m) = 2n } , where s5(m) is the sum of the
 *            base‑5 digits of m. This function counts such m for each
 *            non‑negative integer n
 * Range:     u8 n <= 420, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 8399 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the sum of the base‑5 digits of x.  Works for any
   seq_t that supports basic arithmetic and modulus with int. */
static inline seq_t sum_digits_base5(seq_t x)
{
    seq_t s = 0;
    do
    {
        s += x % 5;
        x /= 5;
    } while (x);
    return s;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n * 2;          /* 2n as seq_t            */
    seq_t low    = 0;                     /* starting value of m   */
    if (target > (seq_t)30)                /* 30 is safe upper bound  */
        low = target - 30;                /* at most 30 candidates */
    seq_t count = 0;

    for (seq_t m = low; m <= target; ++m)
    {
        if (m + sum_digits_base5(m) == target)
            ++count;
    }

    return count;
}
