/*
 * A145300  a(n) is the maximal prime such that if p_n is the n-th prime then
 *          ceiling(sqrt(a(n)*p_n))^2 - a(n)*p_n is a perfect square.
 * https://oeis.org/A145300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   ipow_i32, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145300: a(n) is the maximal prime q such that
 * ceil(sqrt(q * p_n))^2 - q * p_n is a perfect square,
 * where p_n is the n‑th prime.  This implementation
 * computes the value by iterating over primes between
 * p_n and the theoretical upper bound p_n + 2*sqrt(2*p_n)+2. */

#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/next_prime.h"

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
    /* p_n: the n‑th prime (1‑based index) */
    seq_t p = (seq_t)get_prime(n);

    /* Upper bound given by the theorem in the OEIS entry */
    seq_t bound = p + 2 * floor_sqrt((seq_t)(2 * p)) + 2;

    /* best prime found so far (at least p itself satisfies) */
    seq_t best = p;

    /* iterate over primes q >= p and <= bound */
    uint64_t q = (uint64_t)p;          /* current candidate prime (uint64_t) */
    while (1) {
        /* m = q * p */
        seq_t m = (seq_t)q * p;

        /* ceil(sqrt(m)) */
        seq_t r = floor_sqrt(m);
        seq_t t = (r * r == m) ? r : r + 1;

        /* k = t^2 - m and test if k is a perfect square */
        seq_t k = t * t - m;
        seq_t s = floor_sqrt(k);
        if (s * s == k) {
            best = (seq_t)q;
        }

        /* advance to the next prime */
        q = next_prime(q + 1);
        if ((seq_t)q > bound) {
            break;
        }
    }

    return best;
}
