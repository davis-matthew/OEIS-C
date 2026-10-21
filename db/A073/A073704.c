/*
 * A073704  Smallest prime p such that also p-prime(n)*2 is a prime.
 * https://oeis.org/A073704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the smallest prime p such that p and p‑2*prime(n) are prime.  */
/* The function uses an on‑demand prime list and trial division for
   primality tests.  All values are kept in 64‑bit unsigned integers,
   which are safe for n up to the limits used here.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* --------- dynamic prime list ------------------------------------ */
static uint64_t *prime_list   = NULL;   /* array of primes (unsigned 64‑bit)   */
static size_t   prime_count   = 0;      /* number of primes stored            */
static size_t   prime_cap     = 0;      /* allocated capacity                 */

/* Ensure that the prime list contains all primes up to the given
   value 'limit' (inclusive).  */
static void ensure_primes_upto_value(uint64_t limit)
{
    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    }
    while (prime_count == 0 || prime_list[prime_count-1] <= limit) {
        uint64_t cand = (prime_count == 0 ? 2 : prime_list[prime_count-1] + 1);
        if (cand > 2 && (cand & 1) == 0) cand++;   /* make odd if >2   */
        /* trial division using primes already in the list               */
        bool ok = true;
        for (size_t i = 0; i < prime_count; i++) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { ok = false; break; }
        }
        if (ok) {
            if (prime_count == prime_cap) {          /* grow list       */
                prime_cap = prime_cap * 2;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = cand;
        }
        cand += 2;
    }
}

/* Return the n‑th prime (1‑based).  The list is expanded on demand. */
static uint64_t nth_prime(idx_t n)
{
    if (n < 1) return 0;
    ensure_primes_upto_value(0);                  /* init list if needed */
    while (prime_count < (size_t)n) {
        /* produce the next prime using the current list as base     */
        uint64_t cand = prime_list[prime_count-1] + 1;
        if (cand > 2 && (cand & 1) == 0) cand++;
        for (;;) {
            bool ok = true;
            for (size_t i = 0; i < prime_count; i++) {
                uint64_t p = prime_list[i];
                if (p * p > cand) break;
                if (cand % p == 0) { ok = false; break; }
            }
            if (ok) {
                if (prime_count == prime_cap) {
                    prime_cap = prime_cap * 2;
                    prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
                }
                prime_list[prime_count++] = cand;
                break;
            }
            cand += 2;
        }
    }
    return prime_list[n-1];
}

/* Simple integer square root using double conversion (accurate for <1e12). */
static inline uint64_t isqrt_u64(uint64_t x)
{
    return (uint64_t)floor(sqrt((double)x));
}

/* Check whether x is prime using the prime list (expands if necessary). */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    uint64_t lim = isqrt_u64(x);
    ensure_primes_upto_value(lim);            /* need primes up to sqrt(x)    */
    for (size_t i = 0; i < prime_count; i++) {
        uint64_t p = prime_list[i];
        if (p > lim) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Find the first prime greater than 'v'.  The list is expanded if needed. */
static uint64_t first_prime_greater_than(uint64_t v)
{
    /* binary search on the existing list */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= v) lo = mid + 1;
        else hi = mid;
    }
    if (lo == prime_count) {                /* need to generate more      */
        uint64_t cand = (prime_count == 0 ? 2 : prime_list[prime_count-1] + 1);
        if (cand > 2 && (cand & 1) == 0) cand++;
        for (;;) {
            bool ok = true;
            for (size_t i = 0; i < prime_count; i++) {
                uint64_t p = prime_list[i];
                if (p * p > cand) break;
                if (cand % p == 0) { ok = false; break; }
            }
            if (ok) {
                if (prime_count == prime_cap) {
                    prime_cap = prime_cap * 2;
                    prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
                }
                prime_list[prime_count++] = cand;
                if (cand > v) return cand;
            }
            cand += 2;
        }
    }
    return prime_list[lo];
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t pn = nth_prime(n);          /* prime(n)                               */
    uint64_t target = 2 * pn;            /* compute 2*prime(n)                      */

    /* start searching from the first prime greater than target */
    uint64_t candidate = first_prime_greater_than(target);
    for (;;) {
        uint64_t diff = candidate - target;
        if (is_prime_uint64(diff))      /* both candidate and diff are prime    */
            return (seq_t)candidate;
        /* move to next prime */
        uint64_t next = candidate + 1;
        if (next <= 2) next = 3;
        else if ((next & 1) == 0) next++;
        candidate = first_prime_greater_than(next - 1); /* ensures > next-1   */
    }
}
