/*
 * A108216  Number of semiprimes between 10n and 10n + 9.
 * https://oeis.org/A108216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of semiprimes in the interval [10*n, 10*n+9]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root via binary search, returns floor(sqrt(x)) */
static uint32_t isqrt_uint64(uint64_t x) {
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        uint64_t tmp = r + bit;
        if (tmp * tmp <= x)
            r = tmp;
        bit >>= 1;
    }
    return (uint32_t)r;
}

/* Dynamic prime list */
static uint32_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_cnt = 0;

/* Add a prime to the list, expanding if necessary */
static void add_prime(uint32_t p) {
    if (prime_cnt >= prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
        if (!tmp) abort();
        prime_list = tmp;
    }
    prime_list[prime_cnt++] = p;
}

/* Sieve up to 'limit', storing all primes ≤ limit in prime_list */
static void sieve_up_to(uint32_t limit) {
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp) abort();
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            add_prime(i);
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* Ensure prime list covers all primes up to 'need' */
static void ensure_primes(uint32_t need) {
    static uint32_t current_limit = 0;
    if (need <= current_limit) return;
    sieve_up_to(need);
    current_limit = need;
}

/* Prime test using the precomputed primes */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    uint32_t limit = isqrt_uint64(n);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        if (n % prime_list[i] == 0) return false;
    }
    return true;
}

/* Count semiprimes in a single 10‑wide block [low, high] where high = low+9 */
static seq_t count_semiprimes_range(uint64_t low) {
    uint64_t high = low + 9;
    uint32_t prime_limit = isqrt_uint64(high);
    ensure_primes(prime_limit);
    seq_t cnt = 0;
    for (uint64_t x = low; x <= high; ++x) {
        /* Numbers < 4 cannot be semiprime */
        if (x < 4) continue;
        if (is_prime_uint64(x)) continue;          /* prime, not semiprime */
        uint64_t divisor = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > x) break;
            if (x % p == 0) {
                divisor = p;
                break;
            }
        }
        if (!divisor) continue;   /* should not happen, as x is composite */
        uint64_t q = x / divisor;
        if (is_prime_uint64(q)) ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n) {
    uint64_t start = (uint64_t)n * 10ULL;
    return count_semiprimes_range(start);
}
