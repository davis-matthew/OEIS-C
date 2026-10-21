/*
 * A096478  a(n) = A000040(A096477(n)), i.e., prime(a(n)) and prime(a(n)+1)
 *          are twin primes.
 * https://oeis.org/A096478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime index p such that the primes at positions
 *            p and p+1 are twin primes
 * Range:     u8 n <= 13, u16 n <= 698, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Function prototypes provided by the build */
extern bool is_prime(idx_t x);          /* test if integer x is prime */

/* Static data for prime generation and sequence storage */
static uint64_t *prime_list = NULL;   /* holds generated primes (prime numbers) */
static size_t prime_count   = 0;       /* number of primes stored */
static size_t prime_cap     = 0;       /* capacity of prime_list */

static uint64_t *seq_cache = NULL;    /* cache of sequence values (prime indices) */
static size_t seq_len     = 0;         /* number of cached terms */
static size_t seq_cap     = 0;         /* capacity of seq_cache */

/* The candidate number to test for primality */
static uint64_t candidate = 2;

/* Allocate or grow a dynamic array */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed > *cap) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        *ptr = realloc(*ptr, new_cap * elem_size);
        if (!*ptr) exit(EXIT_FAILURE);
        *cap = new_cap;
    }
}

/* Add the next prime to prime_list */
static void add_next_prime(void)
{
    uint64_t n = candidate;
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > n) break;
            if (n % p == 0) { ok = false; break; }
        }
        if (ok) {                /* n is prime */
            ensure_capacity(prime_count + 1, &prime_cap, (void **)&prime_list,
                            sizeof(uint64_t));
            prime_list[prime_count++] = n;
            candidate = n + 1;
            return;
        }
        ++n;
    }
}

/* Test whether a natural number x is a prime index (uses prime_list for divisors) */
static bool is_prime_index(idx_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    /* Initialise static data on first call */
    static bool init = false;
    if (!init) {
        prime_list = NULL;
        prime_cap = 0;
        prime_count = 0;
        seq_cache = NULL;
        seq_cap = 0;
        seq_len = 0;
        candidate = 2;
        init = true;
    }

    /* Generate terms until we reach the requested n */
    while (seq_len < n) {
        add_next_prime();              /* new prime with index = prime_count */
        /* The new prime is at position prime_count (1‑based). */
        if (prime_count >= 2) {
            idx_t p = prime_count - 1; /* candidate index to check (p) */
            if (is_prime_index(p)) {
                uint64_t prev = prime_list[p - 1];   /* prime at index p */
                uint64_t curr = prime_list[p];       /* prime at index p+1 */
                if (curr - prev == 2) {
                    ensure_capacity(seq_len + 1, &seq_cap,
                                    (void **)&seq_cache, sizeof(uint64_t));
                    seq_cache[seq_len++] = p;         /* store the index p */
                }
            }
        }
    }

    /* Return the requested term as seq_t */
    return (seq_t)seq_cache[n - 1];
}
