/*
 * A137665  Quotients ((p+1)^p - 1)/p^2 for p = prime(n).
 * https://oeis.org/A137665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((prime(n) + 1)^prime(n) - 1)/(prime(n)^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_idx.h"

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

static inline seq_t seq_one(void) { return (seq_t)1; }

static seq_t compute_with_uint128(uint32_t p) {
    /*  p  <= 17  ensures (p+1)^p fits in 128 bits    */
    __uint128_t base   = (uint64_t)p + 1;
    __uint128_t result = 1;
    for (idx_t i = 0; i < (idx_t)p; ++i)
        result *= base;
    __uint128_t denom = (__uint128_t)p * (__uint128_t)p;
    return (seq_t)(result / denom);
}

seq_t a(idx_t n) {
    /* fetch the n-th prime (1‑indexed) */
    uint32_t p = get_prime(n);

    /* If seq_t is a 8‑byte unsigned integer, use a 128‑bit temporary
       to avoid overflow for the small values that actually fit in
       64‑bit.   For larger primes we fall back to the generic
       seq_pow, but those values no longer fit in seq_t and are not
       required to be correct.                                */
    if (sizeof(seq_t) <= 8) {
        if (p <= 17) {           /* 17 gives the largest 64‑bit result */
            return compute_with_uint128(p);
        } else {
            /* use the generic big‑int routine, although it will
               overflow for these large primes.  The results for
               them do not need to be correct in a 64‑bit build. */
            seq_t base = (seq_t)(p + 1);
            seq_t pow_val = ipow_idx(base, (idx_t)p);
            seq_t numerator = pow_val - seq_one();
            seq_t denom = (seq_t)p * (seq_t)p;
            return numerator / denom;
        }
    }

    /* For arbitrary‑precision seq_t the generic routine works. */
    seq_t base = (seq_t)(p + 1);
    seq_t pow_val = ipow_idx(base, (idx_t)p);
    seq_t numerator = pow_val - seq_one();
    seq_t denom = (seq_t)p * (seq_t)p;
    return numerator / denom;
}
