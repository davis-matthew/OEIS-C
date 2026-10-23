/*
 * A144100  Numbers k such that k is strictly greater than f(k), where f(k) =
 *          1 if k is prime, 2 * rad(k) if 4 divides k and rad(k) otherwise.
 * https://oeis.org/A144100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = the n-th integer k > f(k) where f(k) = 1 if k is prime
 *            2·rad(k) if 4 divides k rad(k) otherwise rad(k) is the product
 *            of the distinct prime factors of k
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Sieve data structures ---------- */
static size_t spf_limit = 1;          /* largest index with spf computed */
static uint64_t *spf = NULL;          /* spf[i] = smallest prime factor of i (spf[i]==i means i is prime) */
static uint32_t *prime_list = NULL;   /* list of primes discovered so far */
static size_t prime_cnt  = 0;
static size_t prime_cap  = 0;

/* Ensure that spf[1..limit] are computed. Uses trial division with already known primes. */
static void ensure_spf(size_t limit)
{
    if (limit <= spf_limit) return;

    spf = (uint64_t *)realloc(spf, (limit + 1) * sizeof(uint64_t));
    for (size_t i = spf_limit + 1; i <= limit; ++i)
        spf[i] = 0;

    for (size_t i = spf_limit + 1; i <= limit; ++i) {
        /* find smallest prime factor of i */
        uint64_t spf_i = 0;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_list[j];
            uint64_t pp = (uint64_t)p * (uint64_t)p;
            if (pp > i) break;
            if (i % p == 0) {
                spf_i = p;
                break;
            }
        }
        if (spf_i == 0) {
            spf_i = i;                     /* i is prime */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = (uint32_t)i;
        }
        spf[i] = spf_i;
    }
    spf_limit = limit;
}

/* Compute rad(k) using the spf table. */
static seq_t rad_from_spf(uint64_t k)
{
    seq_t r = 1;
    uint64_t x = k;
    while (x > 1) {
        uint64_t p = spf[x];
        r *= (seq_t)p;
        while (x % p == 0)
            x /= p;
    }
    return r;
}

/* ---------- Memoisation of sequence values ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_cache_sz = 0;
static size_t seq_cache_cap = 0;

static void cache_append(seq_t val)
{
    if (seq_cache_sz == seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache_cap = new_cap;
    }
    seq_cache[seq_cache_sz++] = val;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    static uint64_t next_k_to_check = 2;   /* next integer to test (1-indexed) */

    while (seq_cache_sz < (size_t)n) {
        /* Ensure spf table covers the current k. */
        ensure_spf(next_k_to_check);

        uint64_t k = next_k_to_check++;
        bool is_prime = (spf[k] == k);

        if (is_prime) {
            cache_append((seq_t)k);
            continue;
        }

        seq_t rad = rad_from_spf(k);
        seq_t f   = ((k & 0x3u) == 0) ? rad * 2 : rad;
        if ((seq_t)k > f)
            cache_append((seq_t)k);
    }

    return seq_cache[n - 1];
}
