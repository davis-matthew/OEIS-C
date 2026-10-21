/*
 * A073758  Smallest number that is neither a divisor of nor relatively prime
 *          to n, or 0 if no such number exists.
 * https://oeis.org/A073758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite m < n such that gcd(m,n)>1 and n % m ≠
 *            0; if no such m exists then a(n) = 0
 * Range:     u8 n <= 1738, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;      /* n = 1,2,3 -> no candidates */
    if (is_prime(n)) return 0;/* prime n -> all m are coprime */

    for (idx_t m = 2; m < n; ++m) {
        if (is_prime(m)) continue;          /* skip primes */
        seq_t g = gcd((seq_t)n, (seq_t)m);
        if (g == 1) continue;               /* coprime */
        if (n % m == 0) continue;           /* divisor of n */
        return (seq_t)m;                    /* smallest qualifying m */
    }
    return 0;                               /* set empty */
}
