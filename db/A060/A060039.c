/*
 * A060039  Triangle T(n,k) = 1 if k-th prime is a square mod the n-th prime,
 *          otherwise 0, for 1<=k<n.
 * https://oeis.org/A060039
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = 1 if the k‑th prime is a quadratic residue modulo the
 *            r‑th prime, where r and k are obtained from n by flattening the
 *            triangle T(r,k) (offset 2); otherwise 0
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Validated: partial, pass, 105 terms through n = 106 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/isqrt.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

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
    if (n < 2) return 0;            /* pathologically out of range */
    idx_t m = n - 1;                /* 1‑based index inside the flattened triangle */

    /* Find smallest r such that r*(r-1)/2 >= m */
    idx_t s = isqrt(1 + 8 * m);
    idx_t r = (1 + s) / 2;
    if (r * (r - 1) / 2 < m) r++;   /* adjust if necessary */

    idx_t prev = (r - 2) * (r - 1) / 2;   /* entries before row r */
    idx_t k = m - prev;                   /* 1‑based position within row r */

    uint32_t pr = get_prime(r);         /* r‑th prime */
    uint32_t pk = get_prime(k);         /* k‑th prime */

    int jacobi = jacobi_symbol((uint64_t)pk, (uint64_t)pr); /* Legendre symbol (pk|pr) */
    return (seq_t)(jacobi == 1 ? 1 : 0);
}
