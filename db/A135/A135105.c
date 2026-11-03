/*
 * A135105  Digital sum (base the n-th prime) of n^5.
 * https://oeis.org/A135105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n^5 - (prime(n)-1) · Σ_{k≥1} ⌊n^5 / prime(n)^k⌋
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 857 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the digital sum of n^5 in base the n‑th prime.           */
/* Example: a(2) = 4, a(6) = 12                                         */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard against invalid indices (optional) */
    if (n == 0) return (seq_t)0;

    /* Get the n‑th prime.  get_prime returns uint32_t */
    uint32_t p = get_prime(n);

    /* Compute n^5 using unsigned 128‑bit arithmetic to avoid overflow  */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 n5 = n128 * n128 * n128 * n128 * n128;

    /* Sum floor(n^5 / p^k) for k>=1 until p^k > n^5 */
    unsigned __int128 sum_floors = 0;
    unsigned __int128 p128 = p;
    unsigned __int128 power = p128;           /* p^1          */
    while (power <= n5) {
        sum_floors += n5 / power;
        power *= p128;                       /* p^(k+1)      */
    }

    /* Compute the digital sum */
    unsigned __int128 result128 = n5 - (p128 - 1) * sum_floors;

    return (seq_t)result128;                  /* cast to seq_t */
}
