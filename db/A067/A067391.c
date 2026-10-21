/*
 * A067391  a(n) is the least common multiple of numbers in {1,2,3,...,n-1}
 *          which do not divide n.
 * https://oeis.org/A067391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 47, u128 n <= 89,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = lcm of all integers i in [1..n-1] that do NOT divide n.
   The algorithm iterates i from 1 to n-1 computing the cumulative
   least common multiple and, if n is of the form 2·p^k (p prime),
   divides the result by p.  The computation is independent of order
   of calls and works for any n ≥ 1.  */
/* Dependencies: gcd from "../utils/gcd.h"
   Uses only integer arithmetic (no floating point). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

/* test whether m>1 is a pure prime power.
   If so, *p_out is set to the prime and the function returns true. */
static bool is_prime_power(uint64_t m, uint64_t *p_out)
{
    if (m <= 1)
        return false;
    uint64_t p = 0;
    uint64_t tmp = m;
    if (tmp % 2ULL == 0ULL) {
        p = 2ULL;
    } else {
        for (uint64_t d = 3ULL; d * d <= tmp; d += 2ULL) {
            if (tmp % d == 0ULL) {
                p = d;
                break;
            }
        }
    }
    if (p == 0ULL)          /* m is prime itself */
        p = tmp;
    tmp = m;
    while (tmp % p == 0ULL)
        tmp /= p;
    if (tmp != 1ULL)        /* not a pure prime power */
        return false;
    if (p_out)
        *p_out = p;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1U)
        return (seq_t)1;
    seq_t lcm = 1;
    for (idx_t i = 1; i < n; ++i)
        lcm = lcm_seq(lcm, (seq_t)i);

    if ((n & 1ULL) == 0ULL) {        /* n even – check for 2·p^k form */
        uint64_t m = n >> 1ULL;      /* n/2 */
        uint64_t p = 0;
        if (is_prime_power(m, &p)) {
            lcm = lcm / (seq_t)p;   /* divide by the prime factor p */
        }
    }
    return lcm;
}
