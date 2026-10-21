/*
 * A053070  Primes p such that p-6, p and p+6 are consecutive primes.
 * https://oeis.org/A053070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 233, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Purpose:  computes OEIS A053070 – primes p such that p–6, p and p+6 are consecutive primes.  
 * Method:  generates primes by a segmented sieve and keeps the last two primes to
 *          detect a central prime when the gaps to the neighbouring primes are both 6. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLOCK_SIZE 1048576          /* block size for segmented sieve (1 MiB) */
#define SMALL_PRIME_MAX 50000       /* limit for pre‑computed small primes */

/* static state */
static size_t terms_len = 0;          /* number of terms stored */
static size_t terms_cap = 0;          /* capacity of terms array */
static seq_t *terms = NULL;           /* array of sequence terms */

/* last two primes found, used for consecutive‑gap test */
static seq_t prev2Prime = 0;
static seq_t prevPrime = 0;

/* low end of the next unprocessed block */
static uint64_t sieve_low = 2;

/* small primes for sieving */
static uint32_t small_primes[SMALL_PRIME_MAX / 2 + 1];
static size_t small_prime_count = 0;

/* ------------------------------------------------------------------ */
/* pre‑compute all primes up to SMALL_PRIME_MAX                       */
static void init_small_primes(void)
{
    static bool done = false;
    if (done) return;

    bool sieve[SMALL_PRIME_MAX + 1];
    for (uint32_t i = 0; i <= SMALL_PRIME_MAX; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (uint32_t i = 2; i * i <= SMALL_PRIME_MAX; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= SMALL_PRIME_MAX; j += i)
                sieve[j] = false;
        }
    }

    for (uint32_t i = 2; i <= SMALL_PRIME_MAX; ++i)
        if (sieve[i]) small_primes[small_prime_count++] = i;

    done = true;
}

/* ------------------------------------------------------------------ */
/* append a term to the dynamic array                                  */
static void append_term(seq_t v)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 128;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = v;
}

/* ------------------------------------------------------------------ */
/* generate terms until n_needed is reached                           */
static void generate_until(idx_t n_needed)
{
    init_small_primes();

    while (terms_len < n_needed) {
        uint64_t low = sieve_low;
        uint64_t high = low + BLOCK_SIZE;

        /* allocate and zero the composite flag array */
        size_t block_size = (size_t)(high - low);
        char *comp = (char *)calloc(block_size, 1);   /* 0 = prime candidate */

        /* sieve the block with all small primes */
        for (size_t i = 0; i < small_prime_count; ++i) {
            uint32_t p = small_primes[i];
            uint64_t p2 = (uint64_t)p * (uint64_t)p;
            if (p2 > high) break;

            uint64_t start = (low + p - 1) / p * p;
            if (start < p2) start = p2;

            for (uint64_t j = start; j < high; j += p)
                comp[j - low] = 1;
        }

        /* process primes in the block */
        for (uint64_t i = low; i < high; ++i) {
            if (i < 2) continue;
            if (!comp[i - low]) {
                seq_t prime = (seq_t)i;

                if (prev2Prime != 0 && prevPrime != 0 &&
                    (prevPrime - prev2Prime == (seq_t)6) &&
                    (prime - prevPrime == (seq_t)6)) {
                    append_term(prevPrime);   /* prevPrime is the center prime */
                }

                prev2Prime = prevPrime;
                prevPrime = prime;
            }
        }

        free(comp);
        sieve_low = high;    /* move to the next block */
    }
}

/* ------------------------------------------------------------------ */
/* public function required by the build system                        */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined, but safe */
    generate_until(n);
    return terms[n - 1];
}
