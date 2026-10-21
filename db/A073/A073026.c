/*
 * A073026  Primes p such that p-1 has a prime factor q with (p-1)/2 > q >
 *          p^(3/4).
 * https://oeis.org/A073026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   factorize, next_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A073026: primes p such that p-1 has a prime factor q with (p-1)/2 > q > p^(3/4). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/factorize.h"
#include "../utils/seq_pow.h"

/* Test whether a prime p satisfies the sequence condition. */
static bool is_valid_prime(uint64_t p)
{
    uint64_t n = p - 1;

    /* Factor n = p-1.  Store the prime factors (duplicates ignored). */
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factorize(n, primes, exps);

    /* (p-1)/2 as a seq_t value. */
    seq_t half = ((seq_t)n) / 2;

    /* p^3 as a seq_t value, for comparison with q^4. */
    seq_t p3 = seq_pow((seq_t)p, 3);

    for (size_t i = 0; i < cnt; ++i) {
        seq_t q = (seq_t)primes[i];

        /* Need (p-1)/2 > q. */
        if (half <= q)
            continue;

        /* Need q > p^(3/4) ⇔ q^4 > p^3. */
        if (seq_pow(q, 4) > p3)
            return true;
    }
    return false;
}

/* Return the nth term of A073026.  The first term is a(1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* Undefined for index 0; return 0 as a placeholder. */

    idx_t found = 0;
    uint64_t candidate = 1;   /* next_prime will start from 2. */

    while (found < n) {
        candidate = next_prime(candidate);
        if (is_valid_prime(candidate))
            ++found;
    }

    return (seq_t)candidate;
}
