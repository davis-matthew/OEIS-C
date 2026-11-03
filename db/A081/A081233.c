/*
 * A081233  Let p = n-th prime, take smallest solution (x,y) to the Pellian
 *          equation x^2 - p*y^2 = 1 with x and y >= 1; sequence gives value
 *          of x.
 * https://oeis.org/A081233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 28, u64 n <= 58, u128 n <= 171,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 9949 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = the least x ≥ 1 such that
 * x² – p·y² = 1 with p = n‑th prime,  y ≥ 1.
 * The function returns this x for any n ≥ 1. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t fundamental_solution(idx_t p)
{
    seq_t sp = (seq_t)p;                    /* the prime as seq_t  */
    seq_t a0 = floor_sqrt(sp);              /* integer part of √p   */

    seq_t m   = 0;
    seq_t d   = 1;
    seq_t a   = a0;

    seq_t hprev = 1;
    seq_t h     = a;
    seq_t kprev = 0;
    seq_t k     = 1;

    /* Check trivial solution (never happens for primes, but safe) */
    seq_t left  = h * h;
    seq_t right = sp * k * k + 1;
    if (left == right) return h;

    /* Continue with the continued‐fraction convergents */
    while (1) {
        /* next terms of the period:  m → a → d   */
        m = d * a - m;
        seq_t m2 = m * m;
        d = (sp - m2) / d;
        a = (a0 + m) / d;

        /* update the convergents (h/k) */
        seq_t hnew = a * h + hprev;
        seq_t knew = a * k + kprev;
        hprev = h; kprev = k;
        h = hnew;  k = knew;

        /* check for the fundamental solution */
        left  = h * h;
        right = sp * k * k + 1;
        if (left == right) return h;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but never called */
    idx_t prime = get_prime(n);      /* n‑th prime (uint32_t) */
    return fundamental_solution(prime);
}
