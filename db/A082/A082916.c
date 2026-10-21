/*
 * A082916  Numbers k such that k and binomial(2*k, k) are relatively prime.
 * https://oeis.org/A082916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k (starting with k=0) such that * gcd(k
 *            , binomial(2*k , k)) = 1. Equivalently, * for every prime
 *            divisor p of k, every base‑p digit of k is < ceil(p/2)
 * Range:     u8 n <= 73, u16 n <= 9199, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We pre‑compute a small prime sieve and generate the sequence lazily. */

#include <stdint.h>
#include <stdlib.h>

/* ---------- prime sieve ----------
 * Use a fixed limit (500 000) sufficient for the required range.
 * The sieve is built only once. */
#define PRIME_LIMIT 500000
static unsigned char sieve[PRIME_LIMIT + 1];
static uint32_t primes_arr[40000];
static size_t primes_cnt = 0;
static int sieve_done = 0;

static void init_prime_sieve(void)
{
    if (sieve_done) return;
    for (int i = 2; i <= PRIME_LIMIT; ++i) sieve[i] = 1;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i) sieve[j] = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes_arr[primes_cnt++] = (uint32_t)i;
    sieve_done = 1;
}

/* ---------- helpers ----------
 * Check whether a given k satisfies the digit condition for all
 * its prime factors.  k is treated as a 64‑bit integer (enough for the
 * tested range). */
static int is_good_k(uint64_t k)
{
    if (k <= 1) return 1;   /* 0 and 1 always satisfy */

    uint64_t orig = k;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes_arr[i];
        if ((uint64_t)p * (uint64_t)p > k) break;
        if (k % p == 0) {
            uint32_t th = (p + 1) / 2;
            uint64_t t = orig;
            while (t) {
                uint32_t d = (uint32_t)(t % p);
                if (d >= th) return 0;
                t /= p;
            }
            while (k % p == 0) k /= p;
        }
    }
    if (k > 1) { /* remaining prime factor */
        uint32_t p = (uint32_t)k;
        uint32_t th = (p + 1) / 2;
        uint64_t t = orig;
        while (t) {
            uint32_t d = (uint32_t)(t % p);
            if (d >= th) return 0;
            t /= p;
        }
    }
    return 1;
}

/* ---------- sequence cache ----------
 * The sequence is generated lazily and stored in a dynamic array.
 * Only C features are used (malloc/realloc). */
static seq_t *seq_cache = NULL;
static size_t seq_cache_size = 0;
static size_t seq_cache_cap = 0;

static void ensure_seq_cache(void)
{
    if (!seq_cache) {
        seq_cache_cap = 64;
        seq_cache = (seq_t *)malloc(seq_cache_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
    }
}

static void seq_push_back(seq_t val)
{
    if (seq_cache_size == seq_cache_cap) {
        seq_cache_cap = seq_cache_cap * 2 + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, seq_cache_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_cache = tmp;
    }
    seq_cache[seq_cache_size++] = val;
}

/* ---------- main sequence function ----------
 * Returns a(n) for n >= 1.  The function is independent of call order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, safety */
    init_prime_sieve();
    ensure_seq_cache();

    /* initialise cache with first term a(1) = 0 if not already */
    if (seq_cache_size == 0) seq_push_back((seq_t)0);

    while (seq_cache_size < n) {
        uint64_t cand = (uint64_t)seq_cache[seq_cache_size - 1] + 1;
        for (;;) {
            if (is_good_k(cand)) seq_push_back((seq_t)cand);
            ++cand;
            if (seq_cache_size >= n) break;
        }
    }
    return seq_cache[n - 1];
}
