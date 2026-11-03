/*
 * A120450  Number of ways to express a prime p as 2*p1 + 3*p2, where p1, p2
 *          are primes or 1.
 * https://oeis.org/A120450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of representations of the n-th prime p as
 *            2*p1 + 3*p2, where p1 and p2 are primes or 1
 * Range:     u8 n <= 272, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9116 (2026-11-01)
 * Helpers:   build_primes_upto, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_seq.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

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
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt   = 0;
    static uint64_t prime_lim = 0;
    static idx_t   max_n_seen = 0;

    if (n < 1)
        return 0;

    /* Update prime table if a larger index is requested */
    if (n > max_n_seen) {
        uint64_t lim = _prime_limit_for_idx(n);
        if (prime_list) free(prime_list);
        build_primes_upto(lim, &prime_list, &prime_cnt);
        prime_lim = lim;
        max_n_seen = n;
    }

    /* Get the n-th prime */
    uint64_t p = get_prime(n);

    seq_t count = 0;

    /* Check pair where p2 == 1 */
    {
        uint64_t temp = p - 3ULL;         /* 3*1 */
        if (temp > 0 && (temp & 1ULL) == 0) {
            uint64_t p1 = temp >> 1;      /* (p - 3)/2 */
            if (p1 == 1ULL || is_prime_seq(p1))
                ++count;
        }
    }

    /* Iterate over prime p2 values */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p2 = prime_list[i];
        if (p2 > p / 3ULL) break;         /* 3*p2 must be <= p */

        uint64_t temp = p - 3ULL * p2;
        if (temp == 0 || (temp & 1ULL) == 1ULL)
            continue;                    /* cannot be even or non-positive */

        uint64_t p1 = temp >> 1;          /* (p - 3*p2)/2 */
        if (p1 == 1ULL || is_prime_seq(p1))
            ++count;
    }

    return count;
}
