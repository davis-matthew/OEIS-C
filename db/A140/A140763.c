/*
 * A140763  A051838 gives numbers m such that the sum of first m primes
 *          divides the product of the first m primes. This sequence gives
 *          corresponding values of the sum of first m primes.
 * https://oeis.org/A140763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the sum of the first m primes where m is the n‑th index
 *            such that the sum of the first m primes divides their product
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime sieve (incremental trial division) ---------- */
static uint32_t *prime_buf = NULL;   /* array of found primes          */
static size_t prime_cap = 0;         /* allocated space                */
static size_t prime_cnt = 0;         /* number of primes stored        */
static uint32_t candidate = 2;       /* next integer to test for prime */

/* Ensure prime_buf can hold at least n primes */
static void ensure_prime_cap(size_t n) {
    if (prime_cap >= n) return;
    size_t new_cap = prime_cap ? prime_cap : 128;
    while (new_cap < n) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(prime_buf, new_cap * sizeof(uint32_t));
    prime_buf = tmp;
    prime_cap = new_cap;
}

/* Generate the next prime, appending it to prime_buf */
static void add_next_prime(void) {
    while (1) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_buf[i];
            uint64_t prod = (uint64_t)p * (uint64_t)p;
            if (prod > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            ensure_prime_cap(prime_cnt + 1);
            prime_buf[prime_cnt++] = candidate;
            candidate++;
            return;
        }
        candidate++;
    }
}

/* ---------- Sequence cache ---------- */
static seq_t *cache_buf = NULL; /* stored sequence values */
static size_t cache_cap = 0;
static size_t cache_cnt = 0;

/* Ensure cache_buf can hold at least n entries */
static void ensure_cache_cap(size_t n) {
    if (cache_cnt >= n) return;
    size_t new_cap = cache_cap ? cache_cap : 128;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache_buf, new_cap * sizeof(seq_t));
    cache_buf = tmp;
    cache_cap = new_cap;
}

/* ---------- Condition test ---------- */
static bool qualifies(const seq_t sum, const seq_t max_prime) {
    seq_t temp = sum;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_buf[i];
        if ((seq_t)p > max_prime) break;          /* we only need primes up to max_prime */

        seq_t p_seq = (seq_t)p;
        if (p_seq * p_seq > temp) break;          /* p is larger than sqrt(temp) */
        if (temp % p_seq == 0) {
            temp /= p_seq;
            if (temp % p_seq == 0) return false;   /* not square‑free */
        }
    }
    if (temp > (seq_t)1) {                         /* remaining factor is prime */
        if (temp > max_prime) return false;        /* too large to be in product */
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;      /* outside the offset – behaviour undefined */

    ensure_cache_cap((size_t)n);
    static seq_t cumulative_sum = 0; /* sum of primes processed so far */

    /* Generate terms until we reach the requested n */
    while ((idx_t)cache_cnt < n) {
        add_next_prime();                         /* extend prime list */
        uint32_t p = prime_buf[prime_cnt - 1];     /* newest prime (m‑th) */
        cumulative_sum += (seq_t)p;

        if (qualifies(cumulative_sum, (seq_t)p)) {
            cache_buf[cache_cnt++] = cumulative_sum;
        }
    }
    return cache_buf[n - 1];
}
