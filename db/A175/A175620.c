/*
 * A175620  Triangle read by rows: T(n,k) = 2^(prime(n) - k - 1) mod n, 1 <= k
 *          <= n.
 * https://oeis.org/A175620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle read by rows, where
 *            T(r,k) = 2^(prime(r)-k-1) mod r, 1 ≤ k ≤ r
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9896 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Dependencies:  get_prime and mod_pow utilities. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    /* The sequence is 1‑based; indices < 1 are treated as 0. */
    if (n == 0) return 0;

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r  = 0;
    idx_t tri = 0;
    while (tri < n)
    {
        ++r;
        tri += r;                /* tri = r(r+1)/2 */
    }

    /* Position k inside that row (1‑based). */
    idx_t prev_tri = tri - r;   /* sum of lengths of rows < r */
    idx_t k = n - prev_tri;     /* 1 ≤ k ≤ r */

    /* Compute the triangle entry: T(r,k). */
    idx_t p   = get_prime(r);          /* prime(r) */
    idx_t exp = p - k - 1;             /* exponent, always ≥ 0 */

    /* 2^(exp) mod r; result fits in seq_t. */
    return powmod((seq_t)2, exp, (seq_t)r);
}
