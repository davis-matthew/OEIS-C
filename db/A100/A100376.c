/*
 * A100376  a(n) is the largest number x such that for m=n to n+x-1,
 *          A006530(m) increases.
 * https://oeis.org/A100376
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the longest strictly increasing run of greatest
 *            prime factors starting at n. Computed by sequentially checking
 *            gpf values from n upward
 * Range:     u8 n <= 1205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 120 terms through n = 9805 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Greatest prime factor of an unsigned integer */
static seq_t greatest_prime_factor(seq_t value)
{
    seq_t max_factor = 1;
    seq_t n = value;

    for (seq_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            max_factor = d;
            while (n % d == 0) {
                n /= d;
            }
        }
    }
    if (n > 1) {
        max_factor = n;
    }
    return max_factor;
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 2, so n < 2 is outside the defined region. */
    if (n < 2)
        return (seq_t)0;

    seq_t count = 1;                         /* at least the element n itself */
    seq_t prev = greatest_prime_factor((seq_t)n);
    idx_t m = n + 1;

    while (1)
    {
        seq_t curr = greatest_prime_factor((seq_t)m);
        if (curr > prev)                     /* strictly increasing gpf */
        {
            ++count;
            prev = curr;
            ++m;
        }
        else
        {
            break;
        }
    }
    return count;
}
