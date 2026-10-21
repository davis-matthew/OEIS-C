/*
 * A231333  a(n) = lcm_{p is a prime divisor of n} (n/p - 1).
 * https://oeis.org/A231333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm_{p | n, p prime} (n/p - 1) (a(1)=1, a(prime)=0)
 * Range:     u8 n <= 29, u16 n <= 149, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n=1 – no prime divisors, conventionally a(1)=1 */
    if (n == 1) return (seq_t)1;

    /* Factor n by trial division; keep only distinct prime factors. */
    idx_t remaining = n;
    seq_t lcm_val = (seq_t)1;
    bool found = false;

    /* handle divisor 2 */
    if (remaining % (idx_t)2 == 0) {
        found = true;
        seq_t term = ((seq_t)n / (seq_t)2) - (seq_t)1;
        if (term == (seq_t)0) return (seq_t)0;       /* n itself is prime */
        lcm_val = (lcm_val / gcd(lcm_val, term)) * term;
        while (remaining % (idx_t)2 == 0) remaining /= (idx_t)2;
    }

    /* odd divisors up to sqrt(remaining) */
    for (idx_t d = (idx_t)3; d <= remaining / d; d += (idx_t)2) {
        if (remaining % d == 0) {
            found = true;
            seq_t term = ((seq_t)n / (seq_t)d) - (seq_t)1;
            if (term == (seq_t)0) return (seq_t)0;
            lcm_val = (lcm_val / gcd(lcm_val, term)) * term;
            while (remaining % d == 0) remaining /= d;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (remaining > (idx_t)1) {
        found = true;
        seq_t term = ((seq_t)n / (seq_t)remaining) - (seq_t)1;
        if (term == (seq_t)0) return (seq_t)0;
        lcm_val = (lcm_val / gcd(lcm_val, term)) * term;
    }

    /* For completeness; for n>1 this path is unreachable */
    return found ? lcm_val : (seq_t)1;
}
