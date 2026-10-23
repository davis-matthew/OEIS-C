/*
 * A040095  Primes p such that x^3 = 23 has a solution mod p.
 * https://oeis.org/A040095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A040095: primes p such that x^3 = 23 has a solution mod p
 *  Algorithm: generate primes in order and keep those for which 23 is a cubic
 *  residue.  For a prime p, 23 is a cubic residue iff
 *      p = 3          (trivial)
 *   OR  p % 3 == 2    (all non‑trivial residues in this class)
 *   OR  p % 3 == 1  AND 23^((p-1)/3) ≡ 1 (mod p)
 *  The function memoises all found primes so that any call a(n) is fast.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/append_candidate.h"   /* void append_candidate(seq_t **, size_t *, size_t *, seq_t); */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t); */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;   /* array of found primes */
    static size_t cache_len = 0;  /* number of elements stored */
    static size_t cache_cap = 0;  /* current capacity of 'cache' */
    static uint64_t last_checked = 2;  /* next candidate to test */

    /* generate primes until we have at least n of them */
    while (cache_len < n) {
        uint64_t p = last_checked;

        /* update last_checked for next loop */
        last_checked = p + 1;
        /* skip even numbers larger than 2 */
        if (p > 2 && (p & 1) == 0)
            continue;

        /* primality test */
        if (!is_prime(p))
            continue;

        /* test whether 23 is a cubic residue modulo p */
        bool ok;
        if (p == 3) {
            ok = true;
        } else if ((p % 3) == 2) {
            ok = true;
        } else { /* p % 3 == 1 */
            seq_t r = mod_pow((seq_t)23, (p - 1) / 3, (seq_t)p);
            ok = (r == (seq_t)1);
        }

        if (ok)
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);
    }

    return cache[n - 1];
}
