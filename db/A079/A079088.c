/*
 * A079088  Number of divisors of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(((prime(n)+1)*(prime(n+1)+1))/4)
 * Range:     u8 n <= 571, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9206 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* τ(x) is the number of positive divisors of x. */
/* Uses the provided utilities get_prime(idx_t) and divisor_count_u64(uint64_t). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/num_divisors.h"

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
    /* The sequence is defined for n ≥ 1. */
    if (n < 1) return 0;          /* undefined for n = 0, return 0 for safety */

    /* Fetch the n-th and (n+1)-th primes.  get_prime returns uint32_t,
       which is safely cast to uint64_t for the following arithmetic. */
    uint64_t p = (uint64_t)get_prime(n);
    uint64_t q = (uint64_t)get_prime(n + 1);

    /* Compute ((p+1)*(q+1))/4.  The product fits into 64 bits for all
       n in the required range (≤ 10000). */
    uint64_t numerator = (p + 1) * (q + 1);
    uint64_t val = numerator / 4;   /* guaranteed to be integer */

    /* Count divisors using the supplied utility.  The result fits into
       seq_t because τ(val) is at most a few thousand for these sizes. */
    uint64_t d = num_divisors(val);
    return (seq_t)d;
}
