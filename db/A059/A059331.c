/*
 * A059331  Primes p such that x^24 = 2 has no solution mod p, but x^12 = 2
 *          has a solution mod p.
 * https://oeis.org/A059331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that x^24 ≡ 2 (mod p) has no
 *            solution but x^12 ≡ 2 (mod p) has a solution. The test uses the
 *            criterion that x^k ≡ a (mod p) is solvable iff
 *            a^((p‑1)/gcd(k,p‑1)) ≡ 1 (mod p)
 * Range:     u8 n <= 1, u16 n <= 255, u32 n <= 370, u64 n <= 370,
 *            u128 n <= 370, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Euclidean gcd for uint64_t */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0ULL) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Check whether odd prime p satisfies the OEIS condition. */
static bool prime_has_property(uint64_t p)
{
    /* 2^((p-1)/gcd(12,p-1)) ≡ 1  (x^12 = 2 solvable)                */
    uint64_t g12 = gcd_u64(12ULL, p - 1ULL);
    uint64_t exp12 = (p - 1ULL) / g12;
    if (mod_pow((seq_t)2, exp12, (seq_t)p) != (seq_t)1)
        return false;

    /* 2^((p-1)/gcd(24,p-1)) ≠ 1  (x^24 = 2 unsolvable)            */
    uint64_t g24 = gcd_u64(24ULL, p - 1ULL);
    uint64_t exp24 = (p - 1ULL) / g24;
    if (mod_pow((seq_t)2, exp24, (seq_t)p) == (seq_t)1)
        return false;

    return true;
}

/* Dynamic array to store primes with the property. */
static uint64_t *prop_primes   = NULL;
static size_t   prop_cap      = 0;
static size_t   prop_cnt      = 0;

static void ensure_prop_cap(size_t need)
{
    if (prop_cnt + need <= prop_cap)
        return;
    size_t new_cap = (prop_cap == 0) ? 16 : prop_cap * 2;
    while (new_cap < prop_cnt + need)
        new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prop_primes, new_cap * sizeof(uint64_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    prop_primes = tmp;
    prop_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)            /* safety for indices below 1  */
        return (seq_t)0;

    /* Generate primes until we have n property primes. */
    static uint64_t candidate = 3ULL;   /* start with the first odd prime */

    while (prop_cnt < (size_t)n) {
        /* Find the next prime (skip even numbers). */
        while (!is_prime((idx_t)candidate))
            candidate += 2ULL;

        /* Test the property. */
        if (candidate > 2ULL && prime_has_property(candidate)) {
            ensure_prop_cap(1);
            prop_primes[prop_cnt++] = candidate;
        }

        candidate += 2ULL;   /* advance to the next odd number */
    }

    return (seq_t)prop_primes[n - 1];
}
