/*
 * A045966  a(1)=3; if n = Product p_i^e_i, n > 1, then a(n) = Product
 *          p_{i+2}^e_i.
 * https://oeis.org/A045966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(1) = 3; for n>1, a(n) = Π (prime_i+2)^e_i, where n = Π
 *            prime_i^e_i
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8559 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"
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
    if (n == 1U)
        return 3;

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p      = primes[i];
        uint64_t exp    = exps[i];

        /* position of p in the prime list – p is prime */
        seq_t  pi_seq  = prime_pi(p);          /* returns seq_t */
        idx_t  idx     = (idx_t)pi_seq;        /* safe: idx fits in 64 bits */

        /* the prime two positions ahead */
        uint64_t q = get_prime(idx + 2);

        /* multiply the corresponding power */
        seq_t pow = ipow_u64_idx(q, exp);
        res *= pow;
    }
    return res;
}
