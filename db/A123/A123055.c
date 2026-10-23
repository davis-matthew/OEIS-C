/*
 * A123055  Lexicographically earliest sequence of nonprime positive integers
 *          whose first differences represent all prime numbers with exactly
 *          one appearance of each prime.
 * https://oeis.org/A123055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A123055: Lexicographically earliest sequence of composite numbers whose
   differences are consecutive distinct primes.
   Construction: start with 1; repeatedly add the smallest unused prime p
   such that current_term + p is composite.  The array S[0] holds a(1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *S = NULL;          /* sequence values                        */
static size_t S_len = 0;         /* number of terms generated              */
static size_t S_cap = 0;         /* allocated capacity for S               */

static uint32_t *primes = NULL;  /* list of primes encountered so far      */
static uint8_t *prime_used = NULL;/* 1 if prime already used else 0         */
static size_t primes_len = 0;    /* number of primes in list               */
static size_t primes_cap = 0;    /* allocated capacity for primes array    */

/* Append a prime to the primes array */
static void add_prime(uint32_t p)
{
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        primes = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        prime_used = (uint8_t *)realloc(prime_used, new_cap * sizeof(uint8_t));
        primes_cap = new_cap;
    }
    primes[primes_len] = p;
    prime_used[primes_len] = 0;        /* not yet used */
    primes_len++;
}

/* Generate the next prime after the current maximum in the list. */
static uint32_t generate_next_prime(void)
{
    uint32_t candidate = primes_len ? primes[primes_len - 1] + 1 : 2;
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < primes_len; i++) {
            uint32_t q = primes[i];
            if ((uint64_t)q * q > candidate)
                break;
            if (candidate % q == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            add_prime(candidate);
            return candidate;
        }
        candidate++;
    }
}

/* Ensure the S array can hold at least 'need' terms. */
static void ensure_seq_cap(size_t need)
{
    if (need <= S_cap)
        return;
    size_t new_cap = S_cap ? S_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    S = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
    S_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n<1, but guard anyway */
        return 0;

    /* Initialise on first call */
    if (S_len == 0) {
        ensure_seq_cap(16);
        S[0] = (seq_t)1;    /* a(1) = 1 */
        S_len = 1;
    }

    /* Build the sequence up to requested n */
    while ((idx_t)S_len < n) {
        seq_t cur = S[S_len - 1];
        size_t i = 0;
        for (;;) {
            if (i >= primes_len)
                generate_next_prime();

            uint32_t p = primes[i];
            if (prime_used[i]) {  /* already used, skip */
                i++;
                continue;
            }

            seq_t candidate = cur + (seq_t)p;
            if (is_prime_big(candidate)) {   /* candidate is prime, try next p */
                i++;
                continue;
            }

            /* Found the next term: use this prime */
            ensure_seq_cap(S_len + 1);
            S[S_len] = candidate;
            prime_used[i] = 1;
            S_len++;
            break;
        }
    }

    return S[n - 1];   /* a(n) */
}
