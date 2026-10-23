/*
 * A140026  Primes of the form 35x^2+39y^2.
 * https://oeis.org/A140026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 35*x^2 + 39*y^2 a(n) = the nth prime p such
 *            that p = 35*x^2 + 39*y^2 for some integers x,y
 * Range:     u8 n <= 2, u16 n <= 206, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* forward declarations of types supplied by the build system */
extern seq_t a(idx_t n);

/* static data */
static seq_t *repr_primes = NULL;   /* array of primes found */
static size_t repr_cnt   = 0;        /* number of primes stored */
static size_t repr_cap   = 0;        /* capacity of the array */

/* integer sqrt for uint32_t, result fits in uint32_t */
static inline uint32_t isqrt_uint32(uint32_t n) {
    uint32_t low = 0, high = 65536;          /* sqrt(UINT32_MAX) < 65536 */
    while (low < high) {
        uint32_t mid = (low + high) >> 1;
        uint64_t sq = (uint64_t)mid * mid;
        if (sq <= n) low = mid + 1;
        else high = mid;
    }
    return low - 1;
}

/* test whether prime p can be written as 35*x^2 + 39*y^2 */
static inline bool is_repr_form(uint32_t p) {
    /* y >= 1 because the form cannot represent a prime with y=0 */
    for (uint32_t y = 1; ; ++y) {
        uint64_t ysq = (uint64_t)y * y;
        uint64_t sub = 39 * ysq;
        if (sub > p) break;
        uint64_t rem = (uint64_t)p - sub;
        if ((rem % 35) != 0) continue;
        uint64_t q = rem / 35;                 /* should be a perfect square */
        uint32_t r = isqrt_uint32((uint32_t)q);
        if ((uint64_t)r * r == q) return true;
    }
    return false;
}

/* push a new prime into the static array */
static void add_prime(uint32_t p) {
    if (repr_cnt == repr_cap) {
        size_t new_cap = repr_cap ? repr_cap * 2 : 256;
        repr_primes = (seq_t *)realloc(repr_primes, new_cap * sizeof(seq_t));
        repr_cap   = new_cap;
    }
    repr_primes[repr_cnt++] = (seq_t)p;
}

/* rebuild the list of representable primes until we have at least *need* entries
   (the function is called only when more primes are required) */
static void build_for(idx_t need) {
    const uint32_t INITIAL_LIMIT = 30000000;   /* 30 million */
    uint32_t limit = INITIAL_LIMIT;
    while (true) {
        /* sieve */
        char *is_prime = (char *)malloc(limit + 1);
        if (!is_prime) abort();
        memset(is_prime, 1, limit + 1);
        is_prime[0] = is_prime[1] = 0;
        for (uint32_t p = 2; (uint64_t)p * p <= limit; ++p) {
            if (is_prime[p]) {
                for (uint32_t m = p * p; m <= limit; m += p)
                    is_prime[m] = 0;
            }
        }

        /* rebuild representable-primes array fresh */
        free(repr_primes);
        repr_primes = NULL;
        repr_cnt = 0;
        repr_cap = 0;

        for (uint32_t i = 2; i <= limit; ++i) {
            if (!is_prime[i]) continue;
            if (is_repr_form(i))
                add_prime(i);
        }
        free(is_prime);

        if (repr_cnt >= (size_t)need) return;
        /* not enough, double the limit and try again */
        if (limit >= UINT32_MAX / 2) abort();   /* unlikely */
        limit *= 2;
    }
}

/* the function required by the framework */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of range, undefined behavior */
    if (repr_cnt < (size_t)n)
        build_for(n);
    return repr_primes[n - 1];
}
