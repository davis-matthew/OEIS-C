/*
 * A019336  Primes with primitive root 6.
 * https://oeis.org/A019336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p for which 6 is a primitive root modulo p.
 *            A prime p has 6 as a primitive root iff gcd(6,p)=1 and for every
 *            prime q dividing p-1 6^((p-1)/q) ≢ 1 (mod p)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* static storage for the computed sequence */
static seq_t *seq = NULL;      /* array of found terms */
static size_t seq_len = 0;     /* number of elements stored */
static size_t seq_cap = 0;     /* allocated capacity */

/* static storage for primes discovered during the search */
/* used for factoring p-1 */
static uint64_t *primes = NULL;
static size_t prime_len = 0;
static size_t prime_cap = 0;

/* keep track of the current candidate to test */
static uint64_t last_candidate = 1;

/* grow the sequence array when needed */
static void grow_seq(void)
{
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* grow the prime cache array when needed */
static void grow_primes(void)
{
    size_t new_cap = (prime_cap == 0) ? 16 : prime_cap << 1;
    uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes = tmp;
    prime_cap = new_cap;
}

/* test whether 6 is a primitive root modulo prime p (p>3) */
static bool six_is_primitive_root(uint64_t p)
{
    if (p == 2 || p == 3) return false;   /* 6 not coprime with p */

    uint64_t m = p - 1;
    uint64_t tmp = m;
    bool ok = true;
    /* iterate over prime factors of m using the discovered prime list */
    for (size_t i = 0; i < prime_len && primes[i] * primes[i] <= tmp; ++i)
    {
        uint64_t q = primes[i];
        if (m % q == 0)
        {
            /* if 6^((p-1)/q) ≡ 1 (mod p), then p fails */
            if (mod_pow((seq_t)6, m / q, (seq_t)p) == (seq_t)1)
            {
                ok = false;
                break;
            }
            /* remove all occurrences of q for the remaining factor test */
            while (tmp % q == 0) tmp /= q;
        }
    }
    if (ok && tmp > 1)        /* tmp is a prime factor > sqrt(m) */
    {
        if (mod_pow((seq_t)6, m / tmp, (seq_t)p) == (seq_t)1)
            ok = false;
    }
    return ok;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, but return 0 */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n)
    {
        /* iterate to next candidate prime */
        do { ++last_candidate; } while (!is_prime(last_candidate));

        /* store candidate prime in prime cache for future factorizations */
        if (prime_len == prime_cap) grow_primes();
        primes[prime_len++] = last_candidate;

        if (six_is_primitive_root(last_candidate))
        {
            if (seq_len == seq_cap) grow_seq();
            seq[seq_len++] = (seq_t)last_candidate;
        }
    }

    return seq[(size_t)n - 1];
}
