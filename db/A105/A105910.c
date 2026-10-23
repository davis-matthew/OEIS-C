/*
 * A105910  Primes for which -38 is a primitive root.
 * https://oeis.org/A105910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow_mod, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: return the n‑th prime p such that -38 is a primitive root modulo p. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/simple_sieve.h"
#include "../utils/pow_mod.h"

seq_t a(idx_t n)
{
    /* Static state: small prime table for primality testing and factorisation */
    static bool           init        = false;
    static uint32_t      *base_primes = NULL;
    static size_t          base_cnt    = 0;

    /* Initialise once */
    if (!init) {
        /* 50000 is comfortably larger than sqrt of the 10000th such prime.
           It gives a quick factorisation for all candidates. */
        simple_sieve(50000, &base_primes, &base_cnt);
        init = true;
    }

    uint64_t found = 0;      /* how many suitable primes found so far */
    uint64_t p     = 2;      /* current candidate prime */

    while (true) {
        /* Quick primality test using small prime table */
        bool prime = true;
        if (p > 2 && (p & 1ULL) == 0)  /* even numbers >2 are composite */
            prime = false;
        else {
            for (size_t i = 0; i < base_cnt; ++i) {
                uint32_t q = base_primes[i];
                uint64_t sq = (uint64_t)q * (uint64_t)q;
                if (sq > p) break;
                if (p % q == 0) { prime = false; break; }
            }
        }

        if (prime) {
            /* Exclude the trivial cases */
            if (p != 2 && p != 19) {
                /* Test whether -38 is a primitive root modulo p */
                uint64_t a_val = (p - (38ULL % p)) % p;  /* ≡ -38 (mod p) */
                uint64_t m     = p - 1;
                uint64_t temp  = m;
                bool ok        = true;

                for (size_t i = 0; i < base_cnt; ++i) {
                    uint32_t q = base_primes[i];
                    uint64_t sq = (uint64_t)q * (uint64_t)q;
                    if (sq > temp) break;
                    if (temp % q == 0) {
                        uint64_t exp = m / q;
                        if (pow_mod(a_val, exp, p) == 1ULL) {
                            ok = false; break;
                        }
                        while (temp % q == 0) temp /= q;
                    }
                }
                if (ok && temp > 1) {               /* remaining prime factor */
                    uint64_t exp = m / temp;
                    if (pow_mod(a_val, exp, p) == 1ULL)
                        ok = false;
                }
                if (ok) {
                    ++found;
                    if (found == n) return (seq_t)p;
                }
            }
        }

        /* Advance to the next candidate */
        if (p == 2)            p = 3;
        else                    p += 2;          /* skip even numbers */
    }
}
