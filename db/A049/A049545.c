/*
 * A049545  Primes p such that x^13 = 2 has a solution mod p.
 * https://oeis.org/A049545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that x^13 ≡ 2 (mod p). */
/* The algorithm keeps a growing list of primes found so far and
   tests each new prime p:  if (p == 2) true;
   else if ((p-1) % 13 != 0) true;
   else true only if 2^((p-1)/13) ≡ 1 (mod p). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Fast modular exponentiation for seq_t. */
static inline seq_t modpow_seq(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Static storage for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    static uint64_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;

    static uint64_t next_num = 2;      /* next integer to test */

    /* Initialise structures on first call. */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        primes_cap = 32;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        cache_len = primes_len = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        uint64_t p = next_num;
        next_num++;                     /* advance for next iteration */

        /* Primitive sanity check: skip values < 2. */
        if (p < 2) continue;

        /* Test primality using previously found primes. */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t q = primes[i];
            if (q * q > p) break;
            if (p % q == 0) { is_prime = false; break; }
        }

        if (!is_prime) continue;        /* not a prime, skip */

        /* Store the prime for future divisibility tests. */
        if (primes_len == primes_cap) {
            primes_cap *= 2;
            primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
        }
        primes[primes_len++] = p;

        /* Determine if p belongs to the sequence. */
        bool ok;
        if (p == 2) {
            ok = true;
        } else if ((p - 1) % 13 != 0) {
            ok = true;                  /* gcd(13, p-1) == 1 */
        } else {
            uint64_t exp = (p - 1) / 13;
            seq_t val = modpow_seq((seq_t)2, exp, (seq_t)p);
            ok = (val == (seq_t)1);
        }

        if (ok) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
