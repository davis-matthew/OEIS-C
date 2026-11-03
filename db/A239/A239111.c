/*
 * A239111  Smallest Pell number (see A000129) divisible by n-th prime.
 * https://oeis.org/A239111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 18, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 335 (2026-11-01)
 * Helpers:   mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A239111: the smallest Pell number divisible by the n‑th prime. */

#include <stdint.h>
#include <stdlib.h>

#include <stdbool.h>
#include <stddef.h>
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

/* -------------------------------------------------------------------------- */
static seq_t pell_exact(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    struct Mat { seq_t a, b, c, d; };
    struct Mat res = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };
    struct Mat base = { (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)0 };

    while (n) {
        if (n & 1) {
            seq_t na = res.a * base.a + res.b * base.c;
            seq_t nb = res.a * base.b + res.b * base.d;
            seq_t nc = res.c * base.a + res.d * base.c;
            seq_t nd = res.c * base.b + res.d * base.d;
            res.a = na; res.b = nb; res.c = nc; res.d = nd;
        }

        seq_t na = base.a * base.a + base.b * base.c;
        seq_t nb = base.a * base.b + base.b * base.d;
        seq_t nc = base.c * base.a + base.d * base.c;
        seq_t nd = base.c * base.b + base.d * base.d;
        base.a = na; base.b = nb; base.c = nc; base.d = nd;

        n >>= 1;
    }
    return res.b;          /* P(n) */
}

/* -------------------------------------------------------------------------- */
/*  Helper: matrix exponentiation of the Pell matrix, returning P(n).          */

/* -------------------------------------------------------------------------- */
/*  Helper: fast doubling modulo p, returning P(n) mod p.                     */
/* -------------------------------------------------------------------------- */
static inline uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return mulmod(a, b, mod);
}

static uint64_t pell_mod_uint64(idx_t n, uint64_t mod)
{
    struct Mat { uint64_t a, b, c, d; };
    struct Mat res = { 1ULL, 0ULL, 0ULL, 1ULL };
    struct Mat base = { 2ULL % mod, 1ULL % mod, 1ULL % mod, 0ULL };

    while (n) {
        if (n & 1) {
            uint64_t na = (mul_mod_u64(res.a, base.a, mod) +
                           mul_mod_u64(res.b, base.c, mod)) % mod;
            uint64_t nb = (mul_mod_u64(res.a, base.b, mod) +
                           mul_mod_u64(res.b, base.d, mod)) % mod;
            uint64_t nc = (mul_mod_u64(res.c, base.a, mod) +
                           mul_mod_u64(res.d, base.c, mod)) % mod;
            uint64_t nd = (mul_mod_u64(res.c, base.b, mod) +
                           mul_mod_u64(res.d, base.d, mod)) % mod;
            res.a = na; res.b = nb; res.c = nc; res.d = nd;
        }

        uint64_t na = (mul_mod_u64(base.a, base.a, mod) +
                       mul_mod_u64(base.b, base.c, mod)) % mod;
        uint64_t nb = (mul_mod_u64(base.a, base.b, mod) +
                       mul_mod_u64(base.b, base.d, mod)) % mod;
        uint64_t nc = (mul_mod_u64(base.c, base.a, mod) +
                       mul_mod_u64(base.d, base.c, mod)) % mod;
        uint64_t nd = (mul_mod_u64(base.c, base.b, mod) +
                       mul_mod_u64(base.d, base.d, mod)) % mod;
        base.a = na; base.b = nb; base.c = nc; base.d = nd;

        n >>= 1;
    }
    return res.b;          /* P(n) mod p */
}

/* -------------------------------------------------------------------------- */
/*  Comparator for qsort on idx_t values.                                      */
/* -------------------------------------------------------------------------- */
static int cmp_idx(const void *a, const void *b)
{
    unsigned long long aa = *(const unsigned long long *)a;
    unsigned long long bb = *(const unsigned long long *)b;
    return (aa > bb) - (aa < bb);
}

/* -------------------------------------------------------------------------- */
/*  Main function: a(n) = smallest Pell number divisible by the n‑th prime.   */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Get the n‑th prime (1‑indexed). */
    uint32_t p32 = get_prime(n);
    uint64_t p = (uint64_t)p32;

    /* Special case for prime 2. */
    if (p == 2ULL)
        return pell_exact( (idx_t)2 );

    /* Compute the Legendre symbol (2|p). */
    int leg = jacobi_symbol((uint64_t)2, p);
    uint64_t t = p - (uint64_t)leg;          /* t = p – (2|p) */

    /* Enumerate all divisors of t. */
    const size_t MAX_DIV = 256;
    idx_t divs[MAX_DIV];
    size_t divcnt = 0;

    for (idx_t i = 1; i * i <= t; ++i) {
        if (t % i == 0) {
            if (divcnt < MAX_DIV) divs[divcnt++] = i;
            idx_t e = t / i;
            if (i != e && divcnt < MAX_DIV) divs[divcnt++] = e;
        }
    }

    /* Sort divisors ascending. */
    qsort(divs, divcnt, sizeof(idx_t), cmp_idx);

    /* Find the smallest divisor k such that P(k) ≡ 0 (mod p). */
    idx_t k = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        if (pell_mod_uint64(divs[i], p) == 0ULL) {
            k = divs[i];
            break;
        }
    }
    if (k == 0)               /* fallback, should not happen */
        k = t;

    /* Return the exact Pell number P(k). */
    return pell_exact(k);
}
