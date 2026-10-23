/*
 * A165446  Primes p such that (p reversed)+2 is a prime.
 * https://oeis.org/A165446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = prime p such that reverse(p)+2 is prime
 * Range:     u8 n <= 8, u16 n <= 148, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/rev.h"
#include "../utils/is_prime_big.h"

/* Static state for the sequence */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Static state for prime generation */
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static size_t prime_idx = 0;

/* Sieve state */
static uint8_t *sieve = NULL;
static size_t sieve_limit = 0;

/* --------------------------------------------------------------------- */
/* Helper to grow the sequence array                                   */
static void ensure_seq(void) {
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap == 0 ? 64 : seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);               /* out of memory */
        seq_arr = tmp;
        seq_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* Initialise a sieve up to LIMIT and build the prime list             */
static void init_sieve_and_primes(void) {
    if (sieve_limit > 0) return;          /* already initialised */
    const size_t LIMIT = 10000000ULL;     /* 10 million – more than enough for n up to 10000 */
    sieve_limit = LIMIT;

    sieve = (uint8_t *)calloc(sieve_limit + 1, 1);
    if (!sieve) exit(1);

    prime_cap = 700000;                    /* enough for primes up to 10M */
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) exit(1);

    prime_cnt = 0;
    for (size_t i = 2; i <= sieve_limit; ++i) {
        if (!sieve[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (uint32_t)i;
            if (i * i <= sieve_limit) {
                for (size_t j = i * i; j <= sieve_limit; j += i)
                    sieve[j] = 1;
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                  */
seq_t a(idx_t n) {
    if (n == 0) return 0;                   /* offset is 1; n>=1 in proper use */

    init_sieve_and_primes();

    /* Build sequence until we have at least n elements */
    while (seq_len < (size_t)n) {
        if (prime_idx >= prime_cnt) {
            /* Should not happen for n <= 10000 with LIMIT=10M */
            exit(1);
        }
        uint32_t p = prime_list[prime_idx++];
        seq_t rev_p = (seq_t)rev((idx_t)p);
        seq_t candidate = rev_p + (seq_t)2;

        bool is_candidate_prime;
        if (candidate <= (seq_t)sieve_limit) {
            is_candidate_prime = !sieve[(size_t)candidate];
        } else {
            is_candidate_prime = is_prime_big(candidate);
        }

        if (is_candidate_prime) {
            ensure_seq();
            seq_arr[seq_len++] = (seq_t)p;
        }
    }

    return seq_arr[n - 1];
}
