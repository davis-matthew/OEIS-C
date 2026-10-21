/*
 * A215454  a(n) = least positive k such that n^2 divides k!
 * https://oeis.org/A215454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that n^2 divides k! (A215454)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sumpowers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sumpowers.h"

/* Helper: integer square root (floor) */
static inline idx_t int_sqrt(idx_t n) {
    idx_t x = (n < 2) ? n : (idx_t)((double)n * 0.5);
    bool done = false;
    while (!done) {
        idx_t y = (x + n / x) / 2;
        done = (y >= x);
        x = y;
    }
    return x;
}

/* Helper: minimal m for given prime p and exponent E */
static inline seq_t min_m_for_prime(idx_t p, idx_t E) {
    if (E == 0) return (seq_t)1;
    idx_t low = 1;
    __int128 high128 = (__int128)p * E;
    idx_t high = (high128 > (__int128)((idx_t)-1)) ? (idx_t)-1 : (idx_t)high128;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (sumpowers(mid, p) >= E)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}

/* Prime sieve cache */
static uint32_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure primes up to 'limit' are available */
static void ensure_primes(idx_t limit) {
    if (prime_limit >= limit)
        return;
    /* free old cache if any */
    free(prime_cache);
    /* Simple sieve */
    char *is_comp = (char*)calloc((size_t)(limit + 1), 1);
    if (!is_comp) return; /* allocation failure */
    idx_t i, j;
    idx_t lim = limit;
    for (i = 2; i * i <= lim; ++i) {
        if (!is_comp[i]) {
            for (j = i * i; j <= lim; j += i)
                is_comp[j] = 1;
        }
    }
    prime_cache = (uint32_t*)malloc((size_t)lim * sizeof(uint32_t));
    if (!prime_cache) {
        free(is_comp);
        return;
    }
    prime_cnt = 0;
    for (i = 2; i <= lim; ++i) {
        if (!is_comp[i]) {
            prime_cache[prime_cnt++] = (uint32_t)i;
        }
    }
    prime_limit = limit;
    free(is_comp);
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)1;  /* a(1) = 1, and n=0 not used  */
    idx_t sqrt_n = int_sqrt(n);
    ensure_primes(sqrt_n);
    idx_t tmp = n;
    seq_t best = (seq_t)1;
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        idx_t p = prime_cache[idx];
        if (p * p > tmp)
            break;
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            idx_t E = 2 * (idx_t)e;       /* exponent in n^2 */
            seq_t m_req = min_m_for_prime(p, E);
            if (m_req > best)
                best = m_req;
        }
    }
    if (tmp > 1) {
        /* tmp is a prime factor with exponent 1 in n */
        idx_t E = 2;  /* doubled exponent */
        seq_t m_req = min_m_for_prime(tmp, E);
        if (m_req > best)
            best = m_req;
    }
    return best;
}
