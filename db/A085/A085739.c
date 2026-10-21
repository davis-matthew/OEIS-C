/*
 * A085739  Partial sums of A034953(n).
 * https://oeis.org/A085739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A085739 – Partial sums of T(p_k) where p_k is the k‑th prime.
    T(m) = m*(m+1)/2.
    a(1) = T(2) = 3
    a(n) = a(n‑1) + T(p_n)   (p_n is the n‑th prime, offset 1)          */

#include <stdlib.h>
#include <stdint.h>

/* ---------- internal state ---------- */
static uint64_t *prime_cache = NULL;   /* array of primes             */
static seq_t   *sum_cache   = NULL;   /* cumulative sums T(p_i)       */
static size_t  prime_cap   = 0;        /* capacity of prime_cache     */
static size_t  sum_cap     = 0;        /* capacity of sum_cache       */
static idx_t   prime_cnt   = 0;        /* number of primes generated  */
static seq_t   last_sum    = 0;        /* current cumulative sum      */
static uint64_t next_candidate = 2;    /* next number to test primality */

/* ---------- helpers ---------- */

/* Grow prime_cache to hold at least 'needed' elements.  */
static void ensure_prime_cap(size_t needed)
{
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 32;
        while (new_cap < needed) new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_cache = tmp;
        prime_cap   = new_cap;
    }
}

/* Grow sum_cache to hold at least 'needed' elements.  */
static void ensure_sum_cap(size_t needed)
{
    if (sum_cap < needed) {
        size_t new_cap = sum_cap ? sum_cap * 2 : 32;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(sum_cache,
                                      new_cap * sizeof(seq_t));
        if (!tmp) abort();
        sum_cache = tmp;
        sum_cap   = new_cap;
    }
}

/* Test primality of n using already found primes.  */
static int is_prime_candidate(uint64_t n)
{
    if (n < 2) return 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* Generate primes and cumulative sums until we have 'need' primes. */
static void generate_up_to(idx_t need)
{
    while (prime_cnt < need) {
        /* Find next prime */
        while (!is_prime_candidate(next_candidate)) ++next_candidate;
        /* Store prime */
        ensure_prime_cap(prime_cnt + 1);
        prime_cache[prime_cnt] = next_candidate;
        /* Compute triangular number T(p) = p*(p+1)/2 in seq_t */
        seq_t tri = (seq_t)next_candidate * ((seq_t)next_candidate + 1) / 2;
        last_sum += tri;
        /* Store cumulative sum */
        ensure_sum_cap(prime_cnt + 1);
        sum_cache[prime_cnt] = last_sum;
        ++prime_cnt;
        ++next_candidate;
    }
}

/* ---------- public entry ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behaviour below offset is irrelevant */
    generate_up_to(n);                 /* ensure up to n primes have been built */
    return sum_cache[n - 1];           /* a(1) is sum_cache[0] */
}
