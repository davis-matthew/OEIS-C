/*
 * A119644  a(n) = n-th prime from among those primes which are coprime to the
 *          n-th composite.
 * https://oeis.org/A119644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n + ω(composite(n)))
 * Range:     u8 n <= 52, u16 n <= 6539, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ω(m) is the number of distinct prime factors of m.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t n); */
#include "../utils/count_distinct_primes.h" /* size_t count_distinct_primes(uint64_t n); */

/* ---------- static data ---------- */

static uint32_t *prime_list = NULL;   /* prime_list[0] = 2 */
static size_t   prime_cnt  = 0;
static size_t   prime_cap  = 0;
static uint32_t prime_last = 1;       /* last integer examined for primality */

static uint32_t *comp_list  = NULL;   /* comp_list[0] = 4 (first composite) */
static size_t   comp_cnt   = 0;
static size_t   comp_cap   = 0;
static uint32_t comp_last  = 3;       /* last integer examined for composite */

/* ---------- helpers ---------- */

/* Ensure prime_list has room for at least 'need' entries. */
static void ensure_prime_capacity(size_t need)
{
    if (prime_cnt >= need) return;
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        uint32_t *p = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!p) exit(EXIT_FAILURE);
        prime_list = p;
        prime_cap  = new_cap;
    }
}

/* Ensure comp_list has room for at least 'need' entries. */
static void ensure_comp_capacity(size_t need)
{
    if (comp_cnt >= need) return;
    if (comp_cap < need) {
        size_t new_cap = comp_cap ? comp_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        uint32_t *p = (uint32_t *)realloc(comp_list, new_cap * sizeof(uint32_t));
        if (!p) exit(EXIT_FAILURE);
        comp_list = p;
        comp_cap  = new_cap;
    }
}

/* Test whether 'candidate' (> prime_last) is prime.  */
static bool test_prime(uint32_t candidate)
{
    /* Use the existing primality test (handles any index type). */
    return is_prime((idx_t)candidate);
}

/* Fill prime_list until it contains at least 'idx' primes (1‑based). */
static void ensure_primes_up_to_idx(idx_t idx)
{
    if ((idx_t)prime_cnt >= idx) return;

    for (uint32_t cand = prime_last + 1; (idx_t)prime_cnt < idx; ++cand) {
        if (test_prime(cand)) {
            ensure_prime_capacity(prime_cnt + 1);
            prime_list[prime_cnt++] = cand;
        }
        prime_last = cand;
    }
}

/* Fill comp_list until it contains at least 'n' composites. */
static void ensure_composites_upto(idx_t n)
{
    if (comp_cnt >= (size_t)n) return;

    for (uint32_t cand = comp_last + 1; comp_cnt < (size_t)n; ++cand) {
        bool isprime = test_prime(cand);
        if (!isprime) {
            ensure_comp_capacity(comp_cnt + 1);
            comp_list[comp_cnt++] = cand;
        }
        comp_last = cand;
    }
}

/* ---------- sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑range requests */

    /* 1. obtain the n‑th composite (offset 1) */
    ensure_composites_upto(n);
    uint32_t composite = comp_list[n - 1];   /* 0‑based */

    /* 2. compute ω(composite) – distinct prime factors */
    size_t omega = count_distinct_primes((uint64_t)composite);

    /* 3. find the (n + ω)-th prime */
    idx_t prime_index = n + (idx_t)omega;
    ensure_primes_up_to_idx(prime_index);
    uint32_t prime = prime_list[prime_index - 1];

    return (seq_t)prime;   /* cast to seq_t – works for builtin integer types;
                              in big‑integer builds the result type can be
                              constructed from a uint32_t. */
}
