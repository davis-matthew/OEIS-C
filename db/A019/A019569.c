/*
 * A019569  Number of bar segments in a certain way of representing the
 *          integers graphically.
 * https://oeis.org/A019569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(2) = 1; a(n) = s + Σ_{i=1..s} ( a(e_i) + a(k_i -
 *            k_{i-1}) ), with k_0 = 0 where n = ∏_{i=1..s} p(k_i)^{e_i} and
 *            p(k) is the k‑th prime
 * Range:     u8 n <= 9668, u16 n <= 9668, u32 n <= 9668, u64 n <= 9668,
 *            u128 n <= 9668, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ----------  Cache for a(n) values  ---------- */
static seq_t *a_cache      = NULL;
static unsigned char *a_used = NULL;
static size_t a_cap      = 0;

static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;
    a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    a_used  = (unsigned char*)realloc(a_used, new_cap * sizeof(unsigned char));
    if (!a_cache || !a_used) exit(EXIT_FAILURE);
    for (size_t i = a_cap; i < new_cap; ++i) a_used[i] = 0;
    a_cap = new_cap;
}

/* ----------  Prime table  ---------- */
static uint64_t *primes          = NULL;
static size_t   primes_cnt       = 0;
static uint32_t *prime_index_arr = NULL;   /* prime_index_arr[p] = index of p (1‑based), 0 if p not prime */
static size_t   prime_limit      = 0;

static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    free(primes);
    free(prime_index_arr);
    primes = NULL;
    prime_index_arr = NULL;

    size_t sz = (size_t)limit + 1;
    primes = (uint64_t*)malloc(sz * sizeof(uint64_t));
    if (!primes) exit(EXIT_FAILURE);
    prime_index_arr = (uint32_t*)malloc(sz * sizeof(uint32_t));
    if (!prime_index_arr) exit(EXIT_FAILURE);
    memset(primes, 0, sz * sizeof(uint64_t));
    memset(prime_index_arr, 0, sz * sizeof(uint32_t));

    bool *is_composite = (bool*)calloc(sz, sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);

    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!is_composite[i])
        {
            primes[primes_cnt++] = (uint64_t)i;
            prime_index_arr[i] = (uint32_t)(primes_cnt);   /* 1‑based */
            if (i * i <= limit)
            {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    prime_limit = limit;
}

/* ----------  Recursive computation of a(n)  ---------- */
static seq_t rec_a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    ensure_cache(n);
    if (a_used[n]) return a_cache[n];

    ensure_primes(n);

    idx_t tmp = n;
    idx_t prev_k = 0;
    seq_t sum = 0;
    size_t s = 0;

    /* factorising n */
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > (uint64_t)tmp) break;
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            uint32_t k = prime_index_arr[p];
            seq_t sub_a_e   = rec_a(e);
            seq_t sub_a_kdif = rec_a((idx_t)(k - prev_k));
            sum += sub_a_e + sub_a_kdif;
            ++s;
            prev_k = k;
        }
    }
    if (tmp > 1)   /* remaining prime factor */
    {
        uint64_t p = tmp;
        uint32_t k = prime_index_arr[p];
        seq_t sub_a_e   = rec_a(1);   /* e = 1 */
        seq_t sub_a_kdif = rec_a((idx_t)(k - prev_k));
        sum += sub_a_e + sub_a_kdif;
        ++s;
        prev_k = k;
    }

    seq_t res = (seq_t)s + sum;
    a_used[n] = 1;
    a_cache[n] = res;
    return res;
}

/* ----------  Public function  ---------- */
seq_t a(idx_t n)
{
    return rec_a(n);
}
