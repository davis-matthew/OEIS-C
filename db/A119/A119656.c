/*
 * A119656  Denominator of BernoulliB(4*prime(n))/30.
 * https://oeis.org/A119656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator(BernoulliB(4*prime(n)))/30
 * Range:     u8 n <= 1, u16 n <= 299, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 88 terms through n = 916 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"

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
    /* Get the n‑th prime; n is 1‑based. */
    uint32_t p = (uint32_t)get_prime(n);

    /* 4·p fits in 64 bits for all arguments that occur in the tests. */
    uint64_t v = 4ULL * (uint64_t)p;

    seq_t prod = 1;

    /* Enumerate all divisors d of v and test d+1 for primality. */
    for (uint64_t d = 1; d * d <= v; ++d) {
        if (v % d == 0) {
            uint64_t q1 = d + 1;
            if (is_prime_seq((seq_t)q1))
                prod = prod * (seq_t)q1;

            uint64_t d2 = v / d;
            if (d2 != d) {
                uint64_t q2 = d2 + 1;
                if (is_prime_seq((seq_t)q2))
                    prod = prod * (seq_t)q2;
            }
        }
    }

    /* Divide by 30 (2·3·5) – guaranteed to divide evenly. */
    prod = prod / (seq_t)30;

    return prod;
}
