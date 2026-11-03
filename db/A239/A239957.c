/*
 * A239957  a(n) = |{0 < g < prime(n): g is a primitive root modulo prime(n)
 *          of the form k^2 + 1}|.
 * https://oeis.org/A239957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primitive roots g modulo prime(n) of the form
 *            k^2 + 1
 * Range:     u8 n <= 18, u16 n <= 1562, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 8852 (2026-11-01)
 * Helpers:   ipow_i32, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Count the primitive roots of the form k^2+1 modulo the n-th prime. */
static bool is_primitive_root(seq_t g, seq_t p,
                              const uint32_t *factors, size_t fcnt)
{
    /* For each prime divisor q of p-1, check g^((p-1)/q) mod p != 1. */
    uint64_t m = (uint64_t)(p - 1);
    for (size_t i = 0; i < fcnt; ++i) {
        uint32_t q = factors[i];
        uint64_t exp = m / q;
        if (powmod(g, exp, p) == 1)
            return false;
    }
    return true;
}

/* Factor the 64‑bit integer n and store the distinct prime factors in
   the array 'factors' (maximum 20 elements). The number of distinct
   factors is stored in '*fcnt'. */
static void factor_uint64(uint64_t n, uint32_t *factors, size_t *fcnt)
{
    *fcnt = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            factors[(*fcnt)++] = (uint32_t)d;
            while (n % d == 0)
                n /= d;
        }
    }
    if (n > 1)
        factors[(*fcnt)++] = (uint32_t)n;
}

/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Get the n-th prime (1-based). */
    seq_t p = (seq_t)get_prime(n);      /* p is uint32_t, cast to seq_t */
    if (p <= 2) {                       /* primes 2 and 3 are handled quickly */
        /* Only g = 1 works for p = 2, g = 2 works for p = 3 */
        return (seq_t)(n == 1 ? 1 : (n == 2 ? 1 : 0));
    }

    uint32_t factors[20];
    size_t fcnt;
    uint64_t pm1 = (uint64_t)(p - 1);
    factor_uint64(pm1, factors, &fcnt);

    /* Count suitable k. */
    seq_t maxk_seq = floor_sqrt(p - 1);
    idx_t maxk = (idx_t)maxk_seq;

    seq_t count = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k;
        seq_t g = kk + 1;
        if (g >= p)
            continue;
        if (is_primitive_root(g, p, factors, fcnt))
            ++count;
    }
    return count;
}
