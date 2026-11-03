/*
 * A231686  a(n) = Sum_{i=0..n} digsum_9(i)^3, where digsum_9(i) = A053830(i).
 * https://oeis.org/A231686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_9(i)^3
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where digsum_9(i) is the sum of the base‑9 digits of i. */

#include <stdbool.h>  /* for completeness, not strictly needed */

/* local helper: compute the sum of the digits of x in base 9 */
static seq_t digsum9(idx_t x)
{
    seq_t sum = 0;
    while (x > 0)
    {
        seq_t digit = (seq_t)(x % 9);
        sum += digit;
        x /= 9;
    }
    return sum;
}

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t d = digsum9(i);
        seq_t cube = d * d * d;
        total += cube;
    }
    return total;
}
