/*
 * A060282  Periodic part of decimal expansion of reciprocal of n-th prime
 *          (leading 0's omitted).
 * https://oeis.org/A060282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 6, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 97 (2026-11-01)
 * Helpers:   mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the periodic part of 1/prime(n), OEIS A060282
   a(n) = floor(10^τ(n) / prime(n)),  τ(n) = length of the repetend
   of 1/prime(n).  The function uses the nth prime directly (1‑indexed).  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod.h"

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Enumerate distinct prime factors of n (n > 1).  They are stored in
   the caller‑provided array; the caller must provide a buffer of at
   least 64 elements.  Returns the count. */
static int
_factorize_distinct(uint64_t n, uint64_t *factors)
{
    int count = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if ((n % d) == 0) {
            factors[count++] = d;
            while ((n % d) == 0)
                n /= d;
        }
    }
    if (n > 1)
        factors[count++] = n;
    return count;
}

/* Fast modular exponentiation (modulo m) using mul_mod64 to avoid overflow. */
static uint64_t
_modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Multiplicative order of 10 modulo a prime p (p ≠ 2,5).  Returns the
   period length τ(p). */
static uint64_t
_order_of_10(uint64_t p)
{
    uint64_t phi = p - 1;
    uint64_t factors[64];
    int fcnt = _factorize_distinct(phi, factors);
    uint64_t order = phi;
    for (int i = 0; i < fcnt; ++i) {
        uint64_t q = factors[i];
        while (order % q == 0 &&
               _modpow64(10, order / q, p) == 1)
            order /= q;
    }
    return order;
}

/* Public entry point – returns the periodic part for the nth prime. */
seq_t
a(idx_t n)
{
    if (n == 0)           /* below the OEIS offset – undefined behaviour */
        return 0;

    /* n is 1‑indexed: get_prime(1) = 2, get_prime(2) = 3, … */
    uint64_t p = (uint64_t)get_prime(n);

    /* Primes 2 and 5 have terminating decimal expansions. */
    if (p == 2 || p == 5)
        return 0;

    /* Length of the repetend. */
    uint64_t L = _order_of_10(p);

    /* Build the integer made of the L digits of the period. */
    seq_t result = 0;
    uint64_t remainder = 1;
    for (uint64_t i = 0; i < L; ++i) {
        remainder *= 10ULL;
        seq_t digit = (seq_t)(remainder / p);
        remainder   = remainder % p;
        result = mul_small(result, 10) + digit;
    }
    return result;
}
