/*
 * A175217  The second nonprimes after the primes.
 * https://oeis.org/A175217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the second composite number after the n‑th prime
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/* Prime cache                                                  */
static uint64_t *prime_list = NULL;   /* stored as unsigned 64‑bit values */
static size_t prime_cnt  = 0;
static size_t prime_cap  = 0;

static void grow_primes(size_t new_cap)
{
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                 /* allocation failure – unlikely in tests */
    prime_list = tmp;
    prime_cap  = new_cap;
}

static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap)
        grow_primes(prime_cap ? prime_cap * 2 : 8);
    prime_list[prime_cnt++] = p;
}

/* Generate all primes ≤ limit (inclusive) and store them in prime_list. */
static void generate_primes_up_to(uint64_t limit)
{
    if (prime_cnt == 0) {
        add_prime(2);
        if (limit < 2) return;
    }
    uint64_t cand = prime_list[prime_cnt - 1] + 1;
    if (cand % 2 == 0) cand++;   /* after 2 only odd candidates */
    while (cand <= limit) {
        uint64_t root = (uint64_t)floor(sqrt((double)cand));
        bool is_p = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= root; ++i) {
            if (cand % prime_list[i] == 0) { is_p = false; break; }
        }
        if (is_p) add_prime(cand);
        cand += 2;
    }
}

/* Trial‑division primality test that guarantees the prime cache holds
   all primes ≤ sqrt(x). */
static bool is_prime_small(uint64_t x)
{
    if (x < 2) return false;
    generate_primes_up_to((uint64_t)floor(sqrt((double)x)));
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Return the k‑th prime (1‑based).  k ≥ 1. */
static uint64_t get_prime_u64(uint64_t k)
{
    if (k == 0) return 0;
    if (prime_cnt < k) {
        uint64_t cand = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
        if (cand % 2 == 0) cand++;   /* skip evens after 2 */
        while (prime_cnt < k) {
            if (is_prime_small(cand)) add_prime(cand);
            cand += 2;
        }
    }
    return prime_list[k - 1];
}

/* ------------------------------------------------------------------ */
/* Problem solution                                                    */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* behavior below the offset is irrelevant */

    /* 1. Obtain the n‑th prime. */
    uint64_t p = get_prime_u64(n);

    /* 2. Scan upward until the second composite is found. */
    uint64_t composites = 0;
    for (uint64_t x = p + 1; ; ++x) {
        if (!is_prime_small(x)) {
            ++composites;
            if (composites == 2) return (seq_t)x;
        }
    }
}
