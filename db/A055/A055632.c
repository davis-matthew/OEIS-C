/*
 * A055632  Sum of totient function of primes dividing n is a prime.
 * https://oeis.org/A055632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055632: a(n) = n such that the sum of (p-1) over the distinct
   prime divisors p of n is a prime. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build system. */

static idx_t next_test = 2;         /* next integer to test */
static seq_t *seq = NULL;           /* stored sequence values */
static size_t seq_len = 0, seq_cap = 0;

static uint32_t *prime_tbl = NULL;  /* small‑prime table */
static size_t prime_len = 0, prime_cap = 0;

/* Ensure prime_tbl contains all primes up to 'limit' inclusive. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_len && (uint64_t)prime_tbl[prime_len - 1] >= limit) return;

    uint64_t start = (prime_len ? (uint64_t)prime_tbl[prime_len - 1] + 1 : 2);
    for (uint64_t n = start; n <= limit; ++n) {
        bool is_p = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint32_t p = prime_tbl[i];
            if ((uint64_t)p * p > n) break;
            if (n % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_len == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 32;
                prime_tbl = (uint32_t*)realloc(prime_tbl, prime_cap * sizeof(uint32_t));
            }
            prime_tbl[prime_len++] = (uint32_t)n;
        }
    }
}

/* Sum of (p-1) over distinct prime divisors p of x. */
static uint64_t sum_phi_distinct(uint64_t x)
{
    uint64_t sum = 0;
    uint64_t tmp = x;
    for (size_t i = 0; i < prime_len; ++i) {
        uint32_t p = prime_tbl[i];
        if ((uint64_t)p * p > tmp) break;
        if (tmp % p == 0) {
            sum += (uint64_t)p - 1;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) sum += tmp - 1;  /* remaining prime factor */
    return sum;
}

/* Quick primality test using the prime table. */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (size_t i = 0; i < prime_len; ++i) {
        uint32_t p = prime_tbl[i];
        if ((uint64_t)p * p > v) break;
        if (v % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Build the sequence until it contains the n‑th term. */
    while (seq_len < (size_t)n) {
        ensure_primes_up_to(next_test);
        uint64_t s = sum_phi_distinct(next_test);
        if (is_prime_uint64(s)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 32;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)next_test;
        }
        ++next_test;
    }
    return seq[n - 1];
}
