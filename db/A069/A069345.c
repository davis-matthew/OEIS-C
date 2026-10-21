/*
 * A069345  n minus the number of its prime-factors: a(n) = n - A001222(n).
 * https://oeis.org/A069345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - bigomega(n) (prime factors counted with multiplicity)
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1, return 0 as safe guard */

    seq_t omega = bigomega(n);   /* number of prime factors of n (with multiplicity) */
    return (seq_t)n - omega;     /* n minus that count */
}
