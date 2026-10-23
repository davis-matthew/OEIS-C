/*
 * A179858  Least positive primitive root of A139035(n).
 * https://oeis.org/A179858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest primitive root of the n-th prime p for which
 *            2 is a semiprimitive root of p
 * Range:     u8 n <= 10, u16 n <= 84, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start) */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt) */

/* Test whether a prime p (>2) has 2 as a semiprimitive root. */
static bool is_semiprimitive_prime(uint64_t p)
{
    if (p <= 3)                /* 3 fails the conditions below */
        return false;

    uint64_t e = (p - 1) / 2;
    /* e must be odd to avoid -1 in the subgroup */
    if ((e & 1ULL) == 0)
        return false;

    /* 2^e ≡ 1 (mod p) must hold */
    if (mod_pow(2, e, (seq_t)p) != 1)
        return false;

    /* Ensure that the order of 2 is exactly e:
       for every prime factor q of e, 2^(e/q) ≠ 1 (mod p). */
    uint64_t primes[64], exps[64];
    size_t nf;
    factor_u64(e, primes, exps, &nf);
    for (size_t i = 0; i < nf; ++i) {
        uint64_t q = primes[i];
        if (mod_pow(2, e / q, (seq_t)p) == 1)
            return false;
    }
    return true;
}

/* Compute the smallest primitive root of a prime p. */
static seq_t smallest_primitive_root(uint64_t p)
{
    uint64_t primes[64], exps[64];
    size_t nf;
    factor_u64(p - 1, primes, exps, &nf);

    for (uint64_t g = 2; g < p; ++g) {
        bool ok = true;
        for (size_t i = 0; i < nf; ++i) {
            uint64_t q = primes[i];
            if (mod_pow((seq_t)g, (p - 1) / q, (seq_t)p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)g;
    }
    return 0; /* should never happen for a prime */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined, but return 0 */

    /* Cache of precomputed values */
    static seq_t *roots = NULL;
    static size_t roots_cnt = 0;
    static size_t roots_cap = 0;
    static uint64_t last_checked = 2;  /* prime to start searching from */

    /* Grow the cache as needed */
    while (roots_cnt < (size_t)n) {
        /* Find the next prime whose 2 is a semiprimitive root */
        uint64_t p;
        do {
            p = next_prime(last_checked + 1);
            last_checked = p;
        } while (!is_semiprimitive_prime(p));

        /* Compute its smallest primitive root */
        seq_t root = smallest_primitive_root(p);

        /* Grow storage if necessary */
        if (roots_cnt >= roots_cap) {
            size_t new_cap = (roots_cap == 0 ? 16 : roots_cap * 2);
            seq_t *tmp = (seq_t *)realloc(roots, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0; /* allocation failure */
            roots = tmp;
            roots_cap = new_cap;
        }

        roots[roots_cnt++] = root;
    }

    return roots[n - 1];
}
