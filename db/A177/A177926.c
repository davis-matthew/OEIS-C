/*
 * A177926  a(n) = ((prime(n) - 1)!)^2.
 * https://oeis.org/A177926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((prime(n) - 1)!)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External types supplied by the build system */
extern unsigned long long get_prime(idx_t k);

/* Helper: integer square root via trial division (no floating point). */
static uint64_t isqrt_u64(uint64_t n) {
    uint64_t r = 0, bit = 1ULL << 62; /* start with highest power of 4 <= n */
    while (bit > n) bit >>= 2;
    while (bit) {
        uint64_t temp = r + bit;
        if (temp <= n) {
            n -= temp;
            r = temp + bit;
        }
        bit >>= 2;
    }
    return r;
}

/* Dynamic prime list (1‑based index). */
static uint64_t *prime_cache = NULL;   /* array of primes, 0‑based */
static size_t prime_cnt = 0;           /* number of primes stored */

/* Ensure that at least 'needed' primes are available. */
static void ensure_primes(size_t needed) {
    if (prime_cnt >= needed) return;

    /* Start with capacity 16 and grow geometrically. */
    size_t new_cap = prime_cnt ? prime_cnt * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) { /* out of memory, abort */
        exit(EXIT_FAILURE);
    }
    prime_cache = tmp;

    uint64_t cand = (prime_cnt ? prime_cache[prime_cnt - 1] + 1 : 2);

    while (prime_cnt < needed) {
        /* Test cand for primality using existing primes. */
        uint64_t limit = isqrt_u64(cand);
        int is_p = 1;
        for (size_t i = 0; i < prime_cnt && prime_cache[i] <= limit; ++i) {
            if (cand % prime_cache[i] == 0) {
                is_p = 0;
                break;
            }
        }
        if (is_p) {
            prime_cache[prime_cnt++] = cand;
        }
        ++cand;
    }
}

/* Factorial of m (m >= 0), using seq_t arithmetic. */
static seq_t factorial_idx(idx_t m) {
    seq_t f = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        f *= (seq_t)i;
    return f;
}

/* Compute a(n) = ((prime(n) - 1)!)^2. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* defined only for n >= 1 */

    ensure_primes((size_t)n);
    uint64_t prime = prime_cache[n - 1] - 1;   /* (prime(n) - 1) */
    seq_t f = factorial_idx(prime);
    return f * f;
}
