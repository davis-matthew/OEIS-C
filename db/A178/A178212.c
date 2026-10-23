/*
 * A178212  Nonsquarefree numbers divisible by exactly three distinct primes.
 * https://oeis.org/A178212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Nonsquarefree numbers divisible by exactly three distinct primes (OEIS A178212) */
/* Using a simple incremental sieve of primes up to a fixed limit and trial division factorisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool primes_inited = false;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;

/* Initialise a list of primes up to a fixed bound.  The bound is large enough for all
   practical values of n under the timing constraints.  */
static void init_primes(void)
{
    if (primes_inited) return;

    const size_t LIMIT = 200000;   /* sqrt of expected maximum candidate */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();

    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p) is_prime[q] = false;
        }
    }

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap == 0 ? 64 : prime_cap << 1;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = (uint32_t)i;
        }
    }
    free(is_prime);
    primes_inited = true;
}

/* Dynamic array for the sequence. */
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Static variable keeping the next candidate to examine. */
static idx_t next_candidate = 2;

/* Append a value to the dynamic array. */
static void seq_append(seq_t v)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap == 0 ? 64 : seq_cap << 1;
        seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
        if (!seq_vals) abort();
    }
    seq_vals[seq_len++] = v;
}

/* The main function requested by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* Undefined for n=0, but guard. */

    /* Ensure the prime list is ready. */
    init_primes();

    /* Generate terms until the desired index is reached. */
    while (seq_len < n) {
        /* Factor current candidate using the precomputed primes. */
        idx_t value = next_candidate;
        idx_t temp = value;
        size_t distinct = 0;
        bool has_square = false;

        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint32_t p = prime_list[pi];
            if ((uint64_t)p * (uint64_t)p > temp) break;

            if (temp % p == 0) {
                ++distinct;
                int exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                if (exp >= 2) has_square = true;
                if (distinct > 3) break;   /* We cannot be part of the sequence */
            }
        }

        if (distinct <= 3 && temp > 1) {      /* remainder is a prime factor > sqrt */
            ++distinct;
        }

        if (distinct == 3 && has_square) {
            seq_append((seq_t)value);
        }

        ++next_candidate;
    }

    return seq_vals[n - 1];
}
