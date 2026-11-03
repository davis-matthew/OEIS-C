/*
 * A089221  a(n) = floor(sqrt(((prime(n) + primepi(n))/2)^2 +
 *          prime(n)*primepi(n))).
 * https://oeis.org/A089221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor(sqrt(((prime(n) + primepi(n))/2)^2 +
 *            prime(n)*primepi(n)))
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   ipow_i32, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where primepi(n) = π(n) = number of primes ≤ n. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n = 0, return 0 for safety */
        return 0;

    /* nth prime and the prime counting function at n. */
    const seq_t pn   = get_prime(n);   /* prime(n)   */
    const seq_t pi_n = prime_pi(n);  /* primepi(n) = π(n) */

    /* s = p + π(n) */
    const seq_t s = pn + pi_n;

    /* v = (p + π)^2 + 4 * p * π  */
    const seq_t v = s * s + (seq_t)4 * pn * pi_n;

    /* floor(sqrt(v)) gives floor( sqrt( ((p+π)^2)/4 + pπ ) * 2 ) */
    const seq_t sqrtv = floor_sqrt(v);

    /* The desired value is floor(sqrt(v)) / 2. */
    return sqrtv / 2;
}
