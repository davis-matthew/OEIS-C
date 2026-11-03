/*
 * A054512  Log_b 9 where b is smallest primitive root (A001918) mod n-th
 *          prime.
 * https://oeis.org/A054512
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = discrete logarithm base the smallest primitive root of 9
 *            modulo the n-th prime
 * Range:     u8 n <= 54, u16 n <= 6346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9018 (2026-11-01)
 * Helpers:   factorize, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the n-th prime using get_prime, find the smallest primitive root
   of that prime by testing successive integers against the prime factorization
   of (p-1). Then we compute the discrete logarithm by simple repeated
   multiplication modulo p. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factorize.h"
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
    /* get the n-th prime, n >= 5 */
    uint64_t p = (uint64_t)get_prime(n);

    /* factorize p-1 */
    uint64_t fac[64];
    int exp[64];
    int cnt = factorize(p - 1, fac, exp);

    /* find smallest primitive root */
    uint64_t g;
    for (g = 2; ; ++g)
    {
        bool ok = true;
        for (int i = 0; i < cnt && ok; ++i)
        {
            uint64_t q = fac[i];
            seq_t val = powmod((seq_t)g, (p - 1) / q, (seq_t)p);
            if (val == 1)
                ok = false;
        }
        if (ok)
            break;
    }

    /* compute discrete log base g of 9 modulo p */
    seq_t cur = 1;
    seq_t target = (seq_t)9 % (seq_t)p;
    seq_t e = 0;
    while (cur != target)
    {
        cur = (seq_t)((unsigned __int128)cur * g % p);
        ++e;
    }
    return e;
}
