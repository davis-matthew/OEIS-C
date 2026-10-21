/*
 * A220136  Number of ways that a number n can be written as ddd...d where d
 *          is a digit in base b with 1 < b < n-1.
 * https://oeis.org/A220136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 87, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of ways to write n as d repeated k times in base b,
 * where 1 < b < n-1 and 1 <= d <= b-1.  Each triple (b,d,k) counted once.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n < 3 there are no valid bases */
    if (n <= 2) return 0;

    seq_t count = 0;
    seq_t numerator;

    /* Iterate over bases 2 .. n-2 */
    for (idx_t b = 2; b <= n-2; ++b)
    {
        /* numerator = n * (b-1) */
        numerator = (seq_t)n * (seq_t)(b-1);

        /* cur = b^k, start with k=1 (cur = b) */
        seq_t cur = 1;
        while (true)
        {
            /* multiply cur by b */
            cur = cur * (seq_t)b;
            /* denom = b^k - 1 */
            seq_t denom = cur - 1;

            /* If denom > numerator, d would be 0, stop */
            if (denom > numerator) break;

            /* Check if numerator divisible by denom */
            if (numerator % denom == 0)
            {
                seq_t d = numerator / denom;
                if (d >= 1 && d <= (seq_t)(b-1))
                    count += 1;
            }
        }
    }
    return count;
}
