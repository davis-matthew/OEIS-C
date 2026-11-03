/*
 * A169606  a(2n-1) = prime(n+2)-3, a(2n) = prime(n+2)-2.
 * https://oeis.org/A169606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n-1) = prime(n+2)-3, a(2n) = prime(n+2)-2
 * Range:     u8 n <= 106, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 981 (2026-11-01)
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

seq_t a(idx_t n) {
    /* For n < 1 the sequence is undefined. Return 0 for safety. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Determine the index of the needed prime:
       For n = 2k-1 (odd)  -> prime(k+2)
       For n = 2k   (even) -> prime(k+2)
       The common formula is (n + 1) / 2 + 2. */
    idx_t prime_index = (n + 1) / 2 + 2;

    /* Retrieve the k‑th prime (k ≥ 3 here). */
    uint32_t p = get_prime(prime_index);
    seq_t prime = (seq_t)p;

    /* Subtract 3 for odd indices, 2 for even indices. */
    if (n & 1U) {           /* n is odd: a(2k-1) = prime(k+2) - 3 */
        prime -= (seq_t)3;
    } else {                /* n is even: a(2k)   = prime(k+2) - 2 */
        prime -= (seq_t)2;
    }
    return prime;
}
