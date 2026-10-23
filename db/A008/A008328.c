/*
 * A008328  Number of divisors of prime(n)-1.
 * https://oeis.org/A008328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(prime(n)-1) ( divisor count of prime(n)-1 )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array of primes cached across calls */
static uint32_t *primes = NULL;     /* store primes */
static size_t   primes_count = 0;   /* number of primes currently stored */
static size_t   primes_capacity = 0;/* allocated capacity */

/* grow the primes array if needed */
static void ensure_primes_capacity(size_t needed) {
    if (needed <= primes_capacity) return;
    size_t new_cap = primes_capacity ? primes_capacity : 16;
    while (new_cap < needed) new_cap = new_cap * 2 + 1;
    uint32_t *new_ptr = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!new_ptr) exit(EXIT_FAILURE); /* allocation failed */
    primes = new_ptr;
    primes_capacity = new_cap;
}

/* generate the next prime using trial division with known primes */
static uint32_t next_prime(void) {
    uint32_t candidate = primes_count ? primes[primes_count - 1] + 1 : 2;
    for (;;) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > (uint64_t)candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            ensure_primes_capacity(primes_count + 1);
            primes[primes_count++] = candidate;
            return candidate;
        }
        ++candidate;
    }
}

/* ensure we have at least 'needed' primes cached */
static void ensure_primes_count(size_t needed) {
    while (primes_count < needed) next_prime();
}

/* ensure primes are available up to at least sqrt(n) (used for factorization) */
static void ensure_primes_up_to(uint64_t n) {
    /* we need primes up to sqrt(n) */
    while (primes_count == 0 || (uint64_t)primes[primes_count - 1] * primes[primes_count - 1] <= n)
        next_prime();
}

/* compute number of divisors of a 64‑bit integer */
static seq_t divisor_count_u64(uint64_t x) {
    seq_t count = 1;
    uint64_t tmp = x;
    for (size_t i = 0; i < primes_count && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            count = count * (e + 1);
        }
    }
    if (tmp > 1) count = count * 2; /* remaining prime factor */
    return count;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* out of offset, undefined but safe */
    /* make sure we have the first n primes */
    ensure_primes_count((size_t)n);
    uint32_t p = primes[(size_t)n - 1];
    uint64_t m = (uint64_t)p - 1;
    /* factor m using known primes, expanding if necessary */
    ensure_primes_up_to(m);
    return divisor_count_u64(m);
}
