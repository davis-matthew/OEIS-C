/*
 * A211244  Order of 8 mod n-th prime: least k such that prime(n) divides
 *          8^k-1.
 * https://oeis.org/A211244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Order of 8 modulo the n‑th prime. * a(n) = ord_{p_n}(8) =
 *            ord_{p_n}(2) / gcd(3, ord_{p_n}(2)), * where ord_{p}(2) is the
 *            multiplicative order of 2 modulo a prime p
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 998 (2026-11-01)
 * Helpers:   gcd, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/gcd.h"
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
    /* Special case: for p = 2 (i.e., n = 1) the order is defined as 0. */
    if (n == 1)
        return 0;

    seq_t p = (seq_t)get_prime(n);                 /* n‑th prime */
    seq_t ord = p - 1;                             /* initial candidate for ord(2) */

    /* Factorise p-1.  Keep only unique prime factors. */
    uint64_t temp = (uint64_t)(ord);
    uint64_t primes[64];
    size_t pf_cnt = 0;

    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            primes[pf_cnt++] = d;
            while (temp % d == 0)
                temp /= d;
        }
    }
    if (temp > 1)
        primes[pf_cnt++] = temp;

    /* Reduce the order using its prime factors. */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t q = primes[i];          /* prime factor of p-1 */
        while ((ord % (seq_t)q) == 0) {
            uint64_t exp = (uint64_t)(ord / q);
            if (powmod(2, exp, p) == 1)
                ord /= q;
            else
                break;
        }
    }

    /* Compute final answer: ord / gcd(ord, 3). */
    seq_t g = gcd(ord, (seq_t)3);
    return ord / g;
}
