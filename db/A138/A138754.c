/*
 * A138754  a(n) = PrimePi(A138751(n)) - a variation of the Collatz (3n+1)
 *          map.
 * https://oeis.org/A138754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime_pi(next_prime(update_rule(prime(n))))
 * Range:     u8 n <= 33, u16 n <= 4148, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9468 (2026-11-01)
 * Helpers:   next_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where update_rule(p) = 2*p if p == 3 or p ≡ 1 (mod 3),
 *                      otherwise floor(p/2).                     */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/next_prime.h"
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
    /* 1. nth prime (1‑based). */
    uint32_t p = get_prime(n);

    /* 2. Apply the Collatz‑style rule. */
    seq_t m1;
    if (p == 3 || (p % 3) == 1)
        m1 = (seq_t)p * 2;          /* double */
    else
        m1 = (seq_t)p / 2;          /* floor division */

    /* 3. Next prime > m1.  next_prime expects a 64‑bit start value. */
    uint64_t next = next_prime((uint64_t)m1);

    /* 4. Number of primes ≤ next – the required sequence value. */
    return prime_pi(next);
}
