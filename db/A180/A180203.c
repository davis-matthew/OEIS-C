/*
 * A180203  Differences between prime powers of primes, offsetting the prime
 *          and the power by only one. (For purposes of this sequence, 0 and 1
 *          are treated as primes; see Formula.)
 * https://oeis.org/A180203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = EP(n+1)^EP(n) - EP(n)^EP(n-1), where EP(1)=0, EP(2)=1,
 *            EP(k)=Prime(k-2) for k>2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow.h"

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

static inline seq_t EP(idx_t k) {
    if (k == 1) return (seq_t)0;
    if (k == 2) return (seq_t)1;
    return (seq_t)get_prime(k - 2);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined, but harmless */
    if (n == 1) return (seq_t)1;     /* special case */

    seq_t ep_n_minus1 = EP(n - 1);
    seq_t ep_n        = EP(n);
    seq_t ep_n_plus1  = EP(n + 1);

    seq_t term1 = ipow(ep_n_plus1, ep_n);
    seq_t term2 = ipow(ep_n, ep_n_minus1);
    return term1 - term2;
}
