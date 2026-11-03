/*
 * A207336  One half of smallest positive nontrivial even solution of the
 *          congruence x^2 == 1 (mod A001748(n+2)), n>=1.
 * https://oeis.org/A207336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*prime(n+2) - A208296(n))/2. Computed via CRT: the two
 *            non‑trivial solutions of x²≡1 (mod 3*prime(n+2)) are x ≡ 1 (mod
 *            3), x ≡ p‑1 (mod p) and x ≡ 2 (mod 3), x ≡ 1 (mod p), where p =
 *            prime(n+2). The smallest even solution divided by 2 gives a(n)
 * Range:     u8 n <= 95, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 978 (2026-11-01)
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

static uint64_t mod_inv_uint64(uint64_t a, uint64_t m)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)m, newr = (int64_t)a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = newt;
        newt = t - q * newt;
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    /* m is prime, so gcd should be 1 */
    if (r != 1) return 0;           /* no inverse, should not happen */
    if (t < 0) t += (int64_t)m;
    return (uint64_t)t;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    uint64_t p = (uint64_t) get_prime(n + 2);  /* prime(n+2) */
    uint64_t inv3 = mod_inv_uint64(3ULL, p);  /* inverse of 3 mod p */

    /* First non‑trivial solution: x ≡ 1 (mod 3), x ≡ p‑1 (mod p) */
    uint64_t k1 = ((p - 2) * inv3) % p;
    uint64_t x1 = 3 * k1 + 1;

    /* Second non‑trivial solution: x ≡ 2 (mod 3), x ≡ 1 (mod p) */
    uint64_t k2 = ((p - 1) * inv3) % p;
    uint64_t x2 = 3 * k2 + 2;

    /* Choose the smallest even solution */
    bool x1_even = ((x1 & 1ULL) == 0);
    bool x2_even = ((x2 & 1ULL) == 0);

    uint64_t chosen;
    if (x1_even && !x2_even) {
        chosen = x1;
    } else if (!x1_even && x2_even) {
        chosen = x2;
    } else if (x1_even && x2_even) {
        chosen = (x1 < x2 ? x1 : x2);
    } else { /* unlikely: both odd, pick the smaller anyway */
        chosen = (x1 < x2 ? x1 : x2);
    }

    /* Return half of the chosen even solution */
    return (seq_t)(chosen >> 1);
}
