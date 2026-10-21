/*
 * A079880  a(n) = n/mpf(n), where mpf(n) is the median prime factor of n
 *          (A079879).
 * https://oeis.org/A079880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n / mpf(n) where mpf(n) is the median prime factor (with
 *            multiplicity) of n (lower median is used when the number of
 *            prime factors is even)
 * Range:     u8 n <= 451, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static helper to compute integer square root */
static idx_t isqrt_idx(idx_t n) {
    idx_t x = 0;
    idx_t bit = (idx_t)1 << ((sizeof(idx_t)*8 - 2) / 2);
    while (bit) {
        idx_t next = x | bit;
        if (next <= n / next) x = next;
        bit >>= 1;
    }
    return x;
}

/* static storage for primes */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t prime_limit = 0;

static void ensure_primes(idx_t limit) {
    if (limit <= prime_limit) return;
    /* allocate boolean sieve */
    size_t sz = (size_t)limit + 1;
    bool *isprime = (bool *)malloc(sz);
    if (!isprime) return;
    for (size_t i = 0; i < sz; ++i) isprime[i] = true;
    if (sz > 0) isprime[0] = false;
    if (sz > 1) isprime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (isprime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                isprime[j] = false;
        }
    }
    /* count new primes */
    size_t newcnt = primes_cnt;
    for (idx_t i = 2; i <= limit; ++i) if (isprime[i]) ++newcnt;
    primes = (uint32_t *)realloc(primes, newcnt * sizeof(uint32_t));
    size_t idx = primes_cnt;
    for (idx_t i = 2; i <= limit; ++i) if (isprime[i]) primes[idx++] = (uint32_t)i;
    primes_cnt = newcnt;
    prime_limit = limit;
    free(isprime);
}

/* main function to compute a(n) */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;  /* a(1) = 1, a(0) undefined but handled */
    /* factorization */
    idx_t temp = n;
    // Maximum number of prime factors for 64-bit is < 64
    idx_t factors[64];
    idx_t count = 0;

    idx_t limit = isqrt_idx(temp);
    ensure_primes(limit);
    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        uint32_t p = primes[i];
        while (temp % p == 0) {
            if (count < 64) factors[count] = p;
            ++count;
            temp /= p;
        }
    }
    if (temp > 1) {
        if (count < 64) factors[count] = temp;
        ++count;
    }
    /* find median prime factor (lower median) */
    idx_t median = factors[(count - 1) / 2];
    return ((seq_t)n) / ((seq_t)median);
}
