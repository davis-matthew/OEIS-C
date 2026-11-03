/*
 * A023504  Maximum exponent in the prime factorization of prime(n) - 1.
 * https://oeis.org/A023504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9846 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023504: maximum exponent in prime(n)-1 factorization */

/* We compute the n-th prime via the provided helper get_prime.
 * The function then factors p-1 by trial division
 * using 2 and odd divisors up to sqrt(p-1).
 * The largest exponent of any prime factor is returned. */

#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* Handle offset n >= 1; for n == 0 we return 0 (unused). */
    if (n == 0) return 0;

    uint32_t p = get_prime(n);          /* n-th prime (uint32_t) */
    uint64_t m = (uint64_t)p - 1ULL;    /* value to factor */

    seq_t max_exp = 0;

    /* factor out 2 */
    if (m > 1ULL) {
        seq_t cnt = 0;
        while ((m & 1ULL) == 0ULL) {
            m >>= 1;
            ++cnt;
        }
        if (cnt > max_exp) max_exp = cnt;
    }

    /* factor out odd divisors up to sqrt(m) */
    for (uint64_t d = 3ULL; d * d <= m; d += 2ULL) {
        if (m % d == 0ULL) {
            seq_t cnt = 0;
            while (m % d == 0ULL) {
                m /= d;
                ++cnt;
            }
            if (cnt > max_exp) max_exp = cnt;
        }
    }

    /* if remainder > 1, it is a prime factor with exponent 1 */
    if (m > 1ULL && max_exp < 1) {
        max_exp = 1;
    }

    return max_exp;
}
