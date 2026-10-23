/*
 * A173330  First of two intermediate sequences for integral solution of
 *          A002144(n)=x^2+y^2.
 * https://oeis.org/A173330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2k)! / (2·(k!)²) mod p where p = 4k+1 is the n‑th prime
 *            of the form 4k+1
 * Range:     u8 n <= 24, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

static inline seq_t mulmod(seq_t a, seq_t b, seq_t mod)
{
    /* Works for both arithmetic types and arbitrary‑precision types. */
    return (a % mod) * (b % mod) % mod;
}

/* Return the n‑th prime that is congruent to 1 modulo 4. */
static uint64_t prime_4k1(idx_t n)
{
    /* Static data for incremental prime generation. */
    static uint64_t *primes   = NULL;
    static size_t primes_cap  = 0;
    static size_t primes_cnt  = 0;

    static uint64_t *primes4  = NULL;
    static size_t primes4_cap = 0;
    static size_t primes4_cnt = 0;

    static uint64_t next = 3;
    static bool init = false;

    if (!init) {
        primes_cap = 64;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_cnt = 1;

        primes4_cap = 64;
        primes4 = (uint64_t *)malloc(primes4_cap * sizeof(uint64_t));
        primes4_cnt = 0;

        init = true;
    }

    /* Extend the lists until we have found n primes of the form 4k+1. */
    while (primes4_cnt < (size_t)n) {
        /* Primality test using the primes already found. */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= next; ++i) {
            if (next % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Add to the general prime list. */
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = next;

            /* If it is of the form 4k+1, store it separately. */
            if ((next & 3) == 1) {
                if (primes4_cnt == primes4_cap) {
                    primes4_cap <<= 1;
                    primes4 = (uint64_t *)realloc(primes4, primes4_cap * sizeof(uint64_t));
                }
                primes4[primes4_cnt++] = next;
            }
        }
        next += 2; /* Skip even numbers. */
    }
    return primes4[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but harmless */

    /* Find p = 4k+1, the n‑th such prime. */
    uint64_t p = prime_4k1(n);
    seq_t mod = p;
    seq_t k   = (seq_t)((p - 1) / 4);

    /* Compute C(2k, k) modulo p. */
    seq_t num = 1;
    seq_t den = 1;
    for (seq_t i = 1; i <= k; ++i) {
        num = mulmod(num, k + i, mod);
        den = mulmod(den, i, mod);
    }
    seq_t den_inv  = mod_pow(den, (uint64_t)(mod - 2), mod);
    seq_t binom    = mulmod(num, den_inv, mod);

    /* Multiply by inverse of 2 modulo p (p is odd prime). */
    seq_t inv2     = (mod + 1) / 2;   /* (mod + 1)/2 == (p+1)/2 == inverse of 2 modulo p */
    return mulmod(binom, inv2, mod);
}
