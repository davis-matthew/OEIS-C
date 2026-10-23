/*
 * A105961  Primes p such that 20*p + 3 is prime.
 * https://oeis.org/A105961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(n) = the nth prime p such that 20*p + 3 is also prime
 * Range:     u8 n <= 31, u16 n <= 1804, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Helper to generate successive primes using trial division. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0, prime_cap = 0;
static uint64_t prime_candidate = 1;

static uint64_t next_prime(void)
{
    if (prime_cnt == 0) {
        /* first prime 2 */
        if (prime_cap == 0) {
            prime_cap = 16;
            prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_cnt++] = 2ULL;
        return 2ULL;
    }

    while (1) {
        prime_candidate++;
        if (prime_candidate < 3ULL) continue;
        if ((prime_candidate & 1ULL) == 0ULL) continue;  /* even */

        bool is_p = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t pr = prime_list[i];
            if (pr * pr > prime_candidate) break;
            if (prime_candidate % pr == 0ULL) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = prime_candidate;
            return prime_candidate;
        }
    }
}

/* Cache of discovered sequence terms */
static seq_t *seq_vals = NULL;
static size_t seq_cnt = 0, seq_cap = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* behaviour for off‑offset index is irrelevant */

    if (seq_vals == NULL) {
        seq_cap = 16;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_cnt < n) {
        uint64_t p = next_prime();
        uint64_t m = 20ULL * p + 3ULL;          /* 20p+3, fits in uint64_t for required range */
        if (is_prime(m)) {
            seq_t sp = (seq_t)p;
            if (seq_cnt == seq_cap) {
                seq_cap *= 2;
                seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_cnt++] = sp;
        }
    }
    return seq_vals[n - 1];
}
