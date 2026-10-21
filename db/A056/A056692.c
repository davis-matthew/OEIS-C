/*
 * A056692  Number of divisors k of n with gcd(k-1, n) = 1.
 * https://oeis.org/A056692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k | k divides n and gcd(k-1, n) = 1 }
 * Range:     u8 n <= 691, u16 n <= 9643, u32 n <= 9643, u64 n <= 9643,
 *            u128 n <= 9643, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Recursively generate all divisors of n.
 * pos    : index of the current prime factor
 * cur    : current product (partial divisor)
 * n      : the original number being factorised
 * primes : array of distinct prime factors of n
 * exps   : array of exponents of the corresponding primes
 * cnt    : number of distinct prime factors
 * count  : pointer to the running total of valid divisors
 */
static void
generate_divisors (idx_t pos, idx_t cur, idx_t n,
                   const uint64_t *primes, const uint64_t *exps,
                   int cnt, seq_t *count)
{
    if (pos == cnt) {
        /* cur is a divisor of n; check the condition */
        if (gcd ((seq_t)(cur - 1), (seq_t)n) == 1)
            (*count)++;
        return;
    }

    /* Multiply by prime^e for e = 0 .. exps[pos] */
    uint64_t p   = primes[pos];
    idx_t  mul   = 1;
    int    max_e = (int)exps[pos];
    for (int e = 0; e <= max_e; ++e) {
        generate_divisors (pos + 1, cur * mul, n,
                           primes, exps, cnt, count);
        mul *= p;
    }
}

seq_t a (idx_t n)
{
    if (n == 0)          /* undefined for n = 0; return 0 to stay safe */
        return 0;

    /* Prepare arrays for prime factorisation */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   facets = 0;

    factor_u64 ((uint64_t)n, primes, exps, &facets);

    /* Count valid divisors */
    seq_t count = 0;
    generate_divisors (0, 1, n, primes, exps, (int)facets, &count);

    return count;
}
