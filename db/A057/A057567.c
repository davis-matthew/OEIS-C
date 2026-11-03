/*
 * A057567  Number of partitions of n where the product of parts divides n.
 * https://oeis.org/A057567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} A001055(d)
 * Range:     u8 n <= 575, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9925 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n whose product of parts divides n */

#include <stdint.h>

/* Count unordered multiplicative partitions of n using factors >= min_factor.
   Returns the number of factorizations (including the trivial partition {n}). */
static seq_t
count_factorizations(idx_t n, idx_t min_factor)
{
    if (n == 1)            /* 1 has one multiplicative partition: the empty product */
        return (seq_t)1;

    seq_t result = (seq_t)1;   /* the trivial partition {n} */

    /* Iterate over possible first factors.  Using f <= n/f avoids overflow. */
    for (idx_t f = min_factor; f <= n / f; ++f) {
        if (n % f == 0) {
            result += count_factorizations(n / f, f);
        }
    }
    return result;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)    /* undefined for the offset 1, return 0 for safety */
        return (seq_t)0;

    seq_t total = (seq_t)0;

    /* Enumerate all divisors of n in pairs. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            total += count_factorizations(d1, 2);
            if (d1 != d2)
                total += count_factorizations(d2, 2);
        }
    }

    return total;
}
