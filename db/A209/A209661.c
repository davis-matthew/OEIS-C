/*
 * A209661  a(n) = (-1)^A083025(n).
 * https://oeis.org/A209661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, easy, mult
 * Formula:   a(n) = (-1)^A083025(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* integer square root, floor, using only integer arithmetic */
static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t x = 0;
    uint64_t b = 1ULL << 62;             /* largest power of 4 ≤ n */
    while (b > n) b >>= 2;
    while (b) {
        if (n >= x + b) {
            n -= x + b;
            x = (x >> 1) + b;
        } else {
            x >>= 1;
        }
        b >>= 2;
    }
    return x;
}

/* dynamic table of primes generated on demand */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;
static uint64_t prime_last = 1;   /* highest prime currently stored */

/* append a new prime to the table */
static void add_prime(uint64_t p) {
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                            new_cap * sizeof(uint64_t));
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* sieve odd candidates up to a given limit, extending the prime table */
static void sieve_up_to(uint64_t limit) {
    if (limit <= prime_last) return;
    if (prime_last < 2) {
        add_prime(2);
        prime_last = 2;
    }
    uint64_t start = prime_last + 1;
    if ((start & 1) == 0) ++start;          /* first odd number >= start */
    for (uint64_t n = start; n <= limit; n += 2) {
        bool is_p = true;
        uint64_t r = isqrt_u64(n);
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > r) break;
            if (n % p == 0) { is_p = false; break; }
        }
        if (is_p) add_prime(n);
    }
    prime_last = limit;
}

/* main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* behaviour below the offset is irrelevant */

    uint64_t m = n;
    seq_t sign = 1;

    uint64_t limit = isqrt_u64(m);
    sieve_up_to(limit);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit) break;

        if (m % p == 0) {
            unsigned int parity = 0;          /* 0 for even, 1 for odd exponent */
            do {
                m /= p;
                parity ^= 1;
            } while (m % p == 0);

            if ((p & 3) == 1 && parity) {
                sign = -sign;
            }

            if (m == 1) break;
            limit = isqrt_u64(m);
        }
    }

    if (m > 1) {                         /* remaining factor is prime */
        if ((m & 3) == 1) {
            sign = -sign;
        }
    }

    return sign;
}
