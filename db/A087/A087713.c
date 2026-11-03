/*
 * A087713  Greatest prime factor of the product of the neighbors of the n-th
 *          prime.
 * https://oeis.org/A087713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest prime factor of (prime(n)-1)(prime(n)+1)
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9762 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static uint64_t
largest_prime_factor_uint64(uint64_t n) {
    if (n < 2)
        return 1;
    uint64_t largest = 1;
    while ((n & 1) == 0) {          /* factor 2 */
        largest = 2;
        n >>= 1;
    }
    for (uint64_t i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            largest = i;
            n /= i;
        }
    }
    if (n > 1)
        largest = n;                 /* remaining prime factor */
    return largest;
}

seq_t a(idx_t n) {
    /* nth prime (1‑indexed) */
    uint32_t p = get_prime(n);

    /* neighbors of the prime */
    uint64_t left  = (uint64_t)p - 1;
    uint64_t right = (uint64_t)p + 1;

    /* largest prime factors of the neighbors */
    uint64_t g_left  = largest_prime_factor_uint64(left);
    uint64_t g_right = largest_prime_factor_uint64(right);

    /* greatest prime factor of the product */
    return (seq_t)(g_left > g_right ? g_left : g_right);
}
