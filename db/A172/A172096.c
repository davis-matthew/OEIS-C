/*
 * A172096  Primes having no representation of the form prime(n)-+n.
 * https://oeis.org/A172096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A172096: primes p such that p != prime(k)+k and p != prime(k)-k for any k ≥ 1.
   The function builds the list once using a sieve of Eratosthenes up to a fixed bound
   and returns the n‑th term (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global cache for the sequence */
static uint32_t *primes          = NULL;   /* list of primes up to the sieve bound   */
static size_t   prime_cnt        = 0;
static unsigned char *is_prime_arr = NULL; /* sieve flags                            */
static unsigned char *excluded_arr  = NULL; /* primes that can be written as prime(k)±k */
static seq_t *seq_terms          = NULL;   /* cached sequence terms                  */
static size_t   seq_len          = 0;
static size_t   seq_cap          = 0;

/* Build the sieve, primes list, excluded flags and collect the sequence terms. */
static void build_sequence(void)
{
    /* Upper bound chosen large enough to produce at least 10 000 sequence terms. */
    const size_t L = 2000000;

    /* Allocate and initialise sieve array */
    is_prime_arr = (unsigned char*)malloc(L + 1);
    excluded_arr = (unsigned char*)malloc(L + 1);
    for (size_t i = 0; i <= L; ++i) {
        is_prime_arr[i] = 1;
        excluded_arr[i] = 0;
    }
    is_prime_arr[0] = is_prime_arr[1] = 0;

    /* Classic Eratosthenes sieve */
    for (size_t p = 2; (uint64_t)p * p <= L; ++p) {
        if (!is_prime_arr[p]) continue;
        for (size_t mult = p * p; mult <= L; mult += p)
            is_prime_arr[mult] = 0;
    }

    /* Build list of primes */
    size_t cap = 500000;  /* initial capacity, will grow if needed */
    primes = (uint32_t*)malloc(cap * sizeof(uint32_t));
    prime_cnt = 0;
    for (size_t i = 2; i <= L; ++i) {
        if (!is_prime_arr[i]) continue;
        if (prime_cnt == cap) {
            cap *= 2;
            primes = (uint32_t*)realloc(primes, cap * sizeof(uint32_t));
        }
        primes[prime_cnt++] = (uint32_t)i;
    }

    /* Mark primes that can be represented as prime(k)+k or prime(k)-k */
    for (size_t idx = 1; idx <= prime_cnt; ++idx) {
        uint64_t pk = primes[idx - 1];
        uint64_t t1 = pk + idx;                      /* prime(k) + k */
        if (t1 <= L && is_prime_arr[t1]) excluded_arr[t1] = 1;
        uint64_t t2 = pk > idx ? pk - idx : 0;        /* prime(k) - k, avoid underflow */
        if (t2 >= 2 && is_prime_arr[t2]) excluded_arr[t2] = 1;
    }

    /* Collect the sequence terms (primes not excluded) */
    seq_cap = prime_cnt;  /* an upper bound on the number of terms */
    seq_terms = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if (!excluded_arr[p]) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_terms = (seq_t*)realloc(seq_terms, seq_cap * sizeof(seq_t));
            }
            seq_terms[seq_len++] = (seq_t)p;
        }
    }
}

/* Public sequence accessor */
seq_t a(idx_t n)
{
    if (seq_terms == NULL) build_sequence();

    /* n is 1‑based; return 0 for out‑of‑bounds requests (rarely used). */
    if (n == 0 || (size_t)(n - 1) >= seq_len) return 0;
    return seq_terms[n - 1];
}
