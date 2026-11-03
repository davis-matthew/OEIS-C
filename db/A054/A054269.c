/*
 * A054269  Length of period of continued fraction for sqrt(prime(n)).
 * https://oeis.org/A054269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A003285(A000040(n)) – length of the continued‑fraction
 *            period of sqrt(p_n), where p_n is the n‑th prime (a(1)
 *            corresponds to the prime 2). The algorithm uses the standard
 *            recurrence for the continued fraction of sqrt(D), stopping when
 *            a_k = 2·a0. Each call recomputes the period for the req...
 * Range:     u8 n <= 1586, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9620 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Integer square root: floor(sqrt(n)) for 0 <= n <= UINT64_MAX. */
static uint64_t int_isqrt(uint64_t n) {
    uint64_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime.  The `get_prime` utility expects a 1‑based
       index (a(1) --> 2). */
    uint32_t p = get_prime(n);

    /* Integer floor of sqrt(p).  Since p is prime and not a square, a0 > 0. */
    uint64_t a0 = int_isqrt(p);

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_term = a0;
    uint64_t period = 0;

    while (1) {
        uint64_t m_next = d * a_term - m;
        uint64_t d_next = (p - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;
        period++;
        if (a_next == 2 * a0) break;
        m = m_next;
        d = d_next;
        a_term = a_next;
    }
    return (seq_t)period;
}
