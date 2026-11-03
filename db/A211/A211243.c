/*
 * A211243  Order of 7 mod n-th prime: least k such that prime(n) divides
 *          7^k-1.
 * https://oeis.org/A211243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 882 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the multiplicative order of 7 modulo the nth prime
   (a(n) = the least k such that prime(n) divides 7^k - 1;
   if prime(n) = 7 the result is 0) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
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
    /* obtain the nth prime (1‑indexed) */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Special case: prime equals 7 -> order not defined */
    if (p32 == 7U)
        return (seq_t)0;

    /* The order divides p-1.  Compute it by factoring p-1 into distinct primes. */
    uint64_t order = (uint64_t)p32 - 1ULL;
    uint64_t m = order;

    /* array for distinct prime factors of p-1 (size is sufficient for values up to ~1e6) */
    uint64_t factors[16];
    int nf = 0;

    for (uint64_t d = 2ULL; d * d <= m; ++d)
    {
        if (m % d == 0ULL)
        {
            factors[nf++] = d;
            do { m /= d; } while (m % d == 0ULL);
        }
    }
    if (m > 1ULL)
        factors[nf++] = m;

    /* Reduce the order by each distinct prime factor while keeping 7^k ≡ 1 (mod p) */
    for (int i = 0; i < nf; ++i)
    {
        uint64_t f = factors[i];
        while (order % f == 0ULL &&
               powmod((seq_t)7, (idx_t)(order / f), p) == (seq_t)1)
        {
            order /= f;
        }
    }

    return (seq_t)order;
}
