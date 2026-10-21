/*
 * A073871  A073870 divided by n.
 * https://oeis.org/A073871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4, a(2) = a(3) = 3. For n > 3, let m be the odd part of n
 *            (n with all factors of 2 removed). Then a(n) = 2 if m == 1 or m
 *            == 9 or m is prime; otherwise a(n) = 1
 * Range:     u8 n <= 8845, u16 n <= 8845, u32 n <= 8845, u64 n <= 8845,
 *            u128 n <= 8845, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;      /* even number > 2 is not prime */
    idx_t limit = 1;
    while (limit * limit < x) limit++;    /* compute integer sqrt */
    for (idx_t d = 3; d <= limit; d += 2)
    {
        if (x % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2 || n == 3) return (seq_t)3;

    /* Remove all powers of 2 to obtain the odd part. */
    idx_t odd = n;
    while ((odd & 1) == 0)
        odd >>= 1;

    if (odd == 1 || odd == 9) return (seq_t)2;
    if (is_prime_idx(odd))      return (seq_t)2;
    return (seq_t)1;
}
