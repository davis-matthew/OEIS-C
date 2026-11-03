/*
 * A072565  a(n) = prime(n+1)*prime(n+2)+1 mod prime(n), where prime(n) is the
 *          n-th prime.
 * https://oeis.org/A072565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1)*prime(n+2)+1 mod prime(n)
 * Range:     u8 n <= 65, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 85 terms through n = 4758 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* OEIS offset: n >= 1 */
    if (n == 0)
        return 0;   /* undefined for offset, but return 0 */

    /* Retrieve the required primes.  get_prime returns an
       unsigned 32‑bit integer (uint32_t). */
    uint32_t pn   = get_prime(n);
    uint32_t pn1  = get_prime(n + 1);
    uint32_t pn2  = get_prime(n + 2);

    /* Compute (pn1 * pn2 + 1) modulo pn.  Use a 64‑bit intermediate
       to avoid overflow on small seq_t (e.g., 8, 16, 32 bits). */
    uint64_t mod    = (uint64_t)pn;
    uint64_t t1     = (uint64_t)pn1 % mod;
    uint64_t t2     = (uint64_t)pn2 % mod;
    uint64_t prod   = t1 * t2 + 1;
    uint64_t result = prod % mod;

    return (seq_t)result;
}
