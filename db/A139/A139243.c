/*
 * A139243  Second differences of Mersenne numbers A001348, divided by 4.
 * https://oeis.org/A139243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(p_{n+2}) - 2*2^(p_{n+1}) + 2^(p_n)) / 4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 421 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/ipow_u64_idx.h"

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    /* prime indices: first prime = 2 */
    idx_t pn  = (idx_t)get_prime(n);
    idx_t pn1 = (idx_t)get_prime(n + 1);
    idx_t pn2 = (idx_t)get_prime(n + 2);

    idx_t d1 = pn1 - pn;
    idx_t d2 = pn2 - pn1;

    /* small factor S = 2^(d1+d2) - 2*2^d1 + 1 */
    seq_t pow_d1d2 = ipow_u64_idx(2, d1 + d2);
    seq_t pow_d1   = ipow_u64_idx(2, d1);
    seq_t temp     = mul_small(pow_d1, 2);          /* 2 * 2^d1 */
    seq_t S = pow_d1d2 - temp + 1;                 /* subtraction then addition */

    /* base = 2^(p_n - 2) */
    seq_t base = ipow_u64_idx(2, pn - 2);

    /* result = base * S */
    return seq_mul(base, S);
}
