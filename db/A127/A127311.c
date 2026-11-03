/*
 * A127311  a(n) = |E(GF(p))/H| where E(GF(p)) is the group of rational points
 *          on the elliptic curve E: y^2 + y = x^3 - x^2 mod p, the prime p is
 *          prime(n) or prime(n+1) according as n < 5 or n >= 5 and H = {oo,
 *          (0,0), (0,-1), (1,0), (1,-1)}.
 * https://oeis.org/A127311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |E(GF(p))/H| where E: y^2 + y = x^3 - x^2 over GF(p), H =
 *            {∞,(0,0),(0,−1),(1,0),(1,−1)} (|H| = 5). Thus a(n) = |E(GF(p))|
 *            / 5. For n < 5 use the nth prime p = prime(n); for n ≥ 5 use p =
 *            prime(n+1). p ≠ 11 (the curve is singular there)
 * Range:     u8 n <= 50, u16 n <= 6559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9836 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

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

static seq_t count_points(uint64_t p)
{
    /* Count the number of points on y^2 + y = x^3 - x^2 over GF(p). */
    seq_t total = 0;
    for (uint64_t x = 0; x < p; ++x)
    {
        /* v = x^3 - x^2 mod p */
        uint64_t xsq = (x * x) % p;
        uint64_t xv = (x + p - 1) % p;           /* x-1 mod p */
        uint64_t v = (xsq * xv) % p;              /* x^3 - x^2  */
        /* discriminant D = 1 + 4v mod p */
        uint64_t Dtemp = (4 * v) % p;             /* 4v fits in 64 bits here */
        uint64_t D = Dtemp + 1;
        if (D >= p) D -= p;                      /* D mod p */
        int leg = jacobi_symbol(D, p);            /* Legendre symbol */
        total += (seq_t)(1 + leg);                /* 1 + leg(D) points for this x */
    }
    total += (seq_t)1;                            /* point at infinity */
    return total;
}

seq_t a(idx_t n)
{
    /* Determine the appropriate prime index */
    idx_t prime_idx = (n < 5) ? n : n + 1u;
    /* Fetch the prime (uint32_t) and promote */
    uint64_t p = (uint64_t)get_prime(prime_idx);
    /* Compute |E(GF(p))| and divide by |H| = 5 */
    seq_t tot = count_points(p);
    return tot / (seq_t)5;
}
