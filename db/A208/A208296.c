/*
 * A208296  Smallest positive nontrivial odd solution of the congruence x^2 ==
 *          1 (mod A001748(n+2)), n >= 1.
 * https://oeis.org/A208296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(8*T(K(n))+1) where T(x) = x*(x+1)/2 and K(n) =
 *            prime(n+2)-1 if prime(n+2) ≡ 1 (mod 6) prime(n+2) otherwise This
 *            function returns the smallest positive non‑trivial odd solution
 *            of x^2 ≡ 1 (mod A001748(n+2))
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 935 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

/* Integer square root for 64‑bit values without overflow. */
static inline unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long lo = 0, hi = 1ull << 32;   /* 4·10^10 < 2^36 < (1<<32)^2 */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid) lo = mid;
        else                hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n ≥ 1, but no explicit check required */
    uint32_t p = get_prime(n + 2);            /* prime(n+2) */
    uint32_t K = (p % 6 == 1) ? (p - 1) : p;  /* K(n)        */

    unsigned long long k = K;
    /* 8*T(K)+1 = 4*K*(K+1)+1 */
    unsigned long long val = 4ULL * k * (k + 1) + 1;

    unsigned long long root = isqrt_u64(val);
    return (seq_t)root;
}
