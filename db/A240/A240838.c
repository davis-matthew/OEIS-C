/*
 * A240838  Primes p such that prime(p) + 2*p^2 is prime.
 * https://oeis.org/A240838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Primes p such that prime(p) + 2*p^2 is prime
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use incremental prime generation and primality test. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static seq_t *seq = NULL;
static size_t seq_cnt = 0;
static size_t seq_cap = 0;

static uint64_t next_candidate = 3;
static size_t process_idx = 0;

/* Ensure capacity for primes array. */
static void ensure_prime_cap(size_t new_cap) {
    if (primes_cap < new_cap) {
        size_t newsize = primes_cap ? primes_cap * 2 : 64;
        while (newsize < new_cap) newsize <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, newsize * sizeof(uint64_t));
        primes = tmp;
        primes_cap = newsize;
    }
}

/* Ensure capacity for seq array. */
static void ensure_seq_cap(size_t new_cap) {
    if (seq_cap < new_cap) {
        size_t newsize = seq_cap ? seq_cap * 2 : 64;
        while (newsize < new_cap) newsize <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, newsize * sizeof(seq_t));
        seq = tmp;
        seq_cap = newsize;
    }
}

/* Fast primality test for numbers up to current prime list. */
static bool is_prime_candidate(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        __uint128_t mul = (__uint128_t)p * p;
        if (mul > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Initialize data structures once. */
static void init_structs(void) {
    if (primes_cnt == 0) {
        ensure_prime_cap(1);
        primes[0] = 2;
        primes_cnt = 1;
        next_candidate = 3;
        process_idx = 0;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    init_structs();

    /* Generate primes and sequence terms until we have at least n elements. */
    while (seq_cnt < (size_t)n) {
        /* Find next prime. */
        uint64_t q;
        while (true) {
            q = next_candidate;
            ++next_candidate;
            if (is_prime_candidate(q)) {
                ensure_prime_cap(primes_cnt + 1);
                primes[primes_cnt++] = q;
                break;
            }
        }

        /* Process primes as soon as we have enough referenced primes. */
        while (process_idx < primes_cnt) {
            uint64_t p_val = primes[process_idx];
            size_t index = (size_t)p_val;
            if (index > primes_cnt) break;  /* Need more primes to satisfy prime(p). */

            uint64_t prime_of_index = primes[index - 1];
            __uint128_t temp128 = (__uint128_t)prime_of_index + 2 * (__uint128_t)p_val * p_val;
            seq_t temp = (seq_t)temp128;

            if (is_prime_big(temp)) {
                ensure_seq_cap(seq_cnt + 1);
                seq[seq_cnt++] = (seq_t)p_val;
            }
            ++process_idx;
        }
    }

    return seq[n - 1];
}
