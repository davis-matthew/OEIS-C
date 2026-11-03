/*
 * A024678  a(n) is the position of (prime(n+1) + prime(n+2))/2 in the ordered
 *          nonprimes.
 * https://oeis.org/A024678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 4754, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9906 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = position of (prime(n+1)+prime(n+2))/2 in the ordered nonprimes.
   For each n ≥ 1, let m = (p_{n+1}+p_{n+2})/2, where p_k is the k‑th prime.
   Then a(n) = m – π(m), where π(m) counts the primes ≤ m. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/prime_pi.h"

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
    /* n must be ≥ 1; for n = 0 the OEIS offset does not apply. */
    if (n == 0) return 0;

    /* Fetch the consecutive primes p_{n+1} and p_{n+2}. */
    uint32_t p1 = get_prime(n + 1);
    uint32_t p2 = get_prime(n + 2);

    /* Their average is guaranteed integer. */
    seq_t avg = (seq_t)(p1 + p2) / 2;

    /* Convert to idx_t for the prime_pi call (safe for n up to very large values). */
    idx_t m_idx = (idx_t)avg;

    /* Number of primes ≤ avg. */
    seq_t pi = prime_pi(m_idx);

    /* Position among nonprimes: all numbers up to avg minus the primes. */
    return (seq_t)m_idx - pi;
}
