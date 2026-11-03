/*
 * A091284  Exponent of 2 in the prime factorization of prime(n)^8 - 1.
 * https://oeis.org/A091284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = v2(prime(n)-1) + v2(prime(n)+1) + 2 (for n ≥ 2), a(1) =
 *            0, where v2(x) is the exponent of 2 in the factorisation of x
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 9989 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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

static seq_t v2(seq_t x)
{
    /* Count the trailing zeros in the binary representation of x. */
    seq_t cnt = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 1)          /* prime(1) = 2 → 2⁸–1 = 255 is odd */
        return 0;

    /* nth prime (returns a 32‑bit value; cast to seq_t). */
    seq_t p = (seq_t)get_prime(n);

    /* p is odd for all n ≥ 2, so p–1 and p+1 are even. */
    seq_t v1 = v2(p - 1);
    seq_t v2_ = v2(p + 1);

    /* v2(8) = 3; the LTE formula gives v2(p⁸–1) = v2(p–1)+v2(p+1)+v2(8)−1. */
    return v1 + v2_ + 2;            /* 2 = v2(8)−1 */
}
