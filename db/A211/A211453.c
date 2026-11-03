/*
 * A211453  (p-1)/x, where p = prime(n) and x = ord(8,p), the smallest
 *          positive integer such that 8^x == 1 mod p.
 * https://oeis.org/A211453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1)/ord_8_prime(n) (ord is the multiplicative
 *            order of 8 modulo the prime)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 100 terms through n = 975 (2026-11-01)
 * Helpers:   factor_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/powmod.h"

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
    /* special treatment for the first prime 2 */
    uint32_t p = get_prime(n);          /* p is the n-th prime (1‑based) */
    if (p == 2) return 0;              /* (2-1)/ord = 0 because no order exists */

    uint64_t m = (uint64_t)p - 1;        /* p-1 is even for odd primes */

    /* factor m */
    uint64_t primes[64], exps[64];
    size_t    cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* compute multiplicative order of 8 modulo p */
    uint64_t ord = m;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        while (ord % q == 0) {
            uint64_t exp = ord / q;
            if (powmod((seq_t)8, exp, (seq_t)p) == 1)
                ord = exp;
            else
                break;
        }
    }

    /* result as seq_t */
    return ((seq_t)(p - 1)) / ((seq_t)ord);
}
