/*
 * A040995  Primes p such that x^6 = 3 has no solution mod p.
 * https://oeis.org/A040995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, mod_pow, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A040995
 *
 *  a(n) = the nth prime p (p > 3) for which the congruence
 *         x^6 ≡ 3 (mod p) has no solution.
 *  Equivalently, for p > 3,
 *  3^((p‑1)/gcd(6,p‑1)) ≠ 1 (mod p).
 *  The implementation generates primes on demand, checks the residue
 *  condition, stores qualifying primes in a static cache, and returns
 *  the n‑th element.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_realloc.h"

/* static cache of qualifying primes */
static seq_t *cache   = NULL;   /* array of primes satisfying the condition */
static size_t cap     = 0;       /* current capacity of the array        */
static size_t len     = 0;       /* number of elements currently stored  */
static uint64_t last_prime = 1; /* last prime examined */

seq_t a(idx_t n)
{
    while (len < n)
    {
        /* Find next prime after the previous one */
        last_prime = next_prime(last_prime + 1);

        /* Skip p = 3 because 3 ≡ 0 (mod 3) gives the trivial solution x ≡ 0. */
        if (last_prime == 3)
            continue;

        uint64_t p_minus_1 = last_prime - 1;
        uint64_t g = (uint64_t)gcd(6ULL, p_minus_1);
        uint64_t exp = p_minus_1 / g;

        /* Test whether 3 is a sixth‑power residue modulo p */
        seq_t residue = mod_pow((seq_t)3, exp, (seq_t)last_prime);

        /* If residue ≠ 1, the prime satisfies the condition */
        if (residue != (seq_t)1)
        {
            /* Grow cache if necessary */
            if (len == cap)
            {
                size_t new_cap = (cap == 0 ? 4 : cap * 2);
                seq_t *tmp = seq_realloc(cache, cap, new_cap);
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)last_prime;
        }
    }

    return cache[n - 1];
}
