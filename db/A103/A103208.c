/*
 * A103208  Numbers k such that 3 divides prime(1) + ... + prime(k).
 * https://oeis.org/A103208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the k‑th integer k for which the sum of the first k primes
   is divisible by 3.  The k values are stored in an array and
   generated on demand, so calls to a(n) may occur in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static state -----------------------------------------------------------*/
static seq_t *results = NULL;     /* Found k values (a(n))   */
static idx_t  res_count = 0;      /* Number of stored results */
static uint64_t *primes = NULL;   /* Generated primes         */
static size_t   prime_cnt = 0;    /* Number of primes found   */
static size_t   prime_cap = 0;    /* Allocated capacity       */
static seq_t    sum_mod3 = 0;     /* Cumulative sum modulo 3 */
static uint64_t next_candidate = 2; /* Next number to test */

/* Ensure 'results' has enough space for at least one more element */
static void ensure_res_capacity(void)
{
    static size_t res_cap = 0;
    if (res_cap == 0) {
        res_cap = 64;
        results = (seq_t*)malloc(res_cap * sizeof(seq_t));
    } else if (res_count >= res_cap) {
        res_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(results, res_cap * sizeof(seq_t));
        if (!tmp) return;           /* allocation failure – unlikely */
        results = tmp;
    }
}

/* Ensure 'primes' has enough space for at least one more element */
static void ensure_prime_capacity(void)
{
    if (prime_cap == 0) {
        prime_cap = 64;
        primes = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_cnt >= prime_cap) {
        prime_cap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(primes, prime_cap * sizeof(uint64_t));
        if (!tmp) return;
        primes = tmp;
    }
}

/* Simple primality test using trial division by already known primes. */
static bool is_prime_candidate(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > x / p) break;         /* p*p > x, safe from overflow */
        if (x % p == 0) return false;
    }
    return true;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Generate results up to the requested index 'n'. */
    while (res_count < n) {
        /* Find the next prime */
        uint64_t p = next_candidate;
        while (!is_prime_candidate(p)) {
            p += (p == 2) ? 1 : 2;   /* advance: 2->3, then odd steps */
        }

        /* Store the found prime */
        ensure_prime_capacity();
        primes[prime_cnt++] = p;
        next_candidate = (p == 2) ? 3 : p + 2;

        /* Update cumulative sum modulo 3 */
        sum_mod3 = (seq_t)((sum_mod3 + (seq_t)(p % 3)) % 3);
        if (sum_mod3 == 0) {
            /* Current prime is the k‑th prime where the sum is 0 mod 3. */
            ensure_res_capacity();
            results[res_count++] = (seq_t)prime_cnt;
        }
    }
    return results[n - 1];
}
