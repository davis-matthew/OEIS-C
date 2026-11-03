/*
 * A048767  If n = Product (p_j^k_j) then a(n) = Product ( prime(k_j)^pi(p_j)
 *          ) where pi is A000720.
 * https://oeis.org/A048767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} prime(e)^π(p) (π is the prime‑counting
 *            function)
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 130, u64 n <= 310,
 *            u128 n <= 718, big any n
 * Validated: standard, pass, 74 terms through n = 9485 (2026-11-01)
 * Helpers:   factor_u64, ipow, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
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
    if (n <= 1)
        return (seq_t)1;

    /* Maximum number of distinct prime factors of a 64‑bit integer is 16. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pcnt = 0;

    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t base = (seq_t)get_prime((idx_t)e);        /* prime(e)  */
        seq_t exp  = prime_pi((idx_t)p);              /* π(p)      */

        result = result * ipow(base, exp);
    }
    return result;
}
