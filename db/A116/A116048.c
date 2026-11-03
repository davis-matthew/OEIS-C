/*
 * A116048  sigma(n) times the n-th prime gives a triangular number.
 * https://oeis.org/A116048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116048:  k for which prime(k) * sigma(k) is a triangular number.
   The code searches indices k in increasing order, storing those that satisfy
   the condition to answer any query a(n). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

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

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
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

seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static idx_t   curr   = 0;   /* last candidate k examined   */
    static seq_t  *found  = NULL;/* array of indices that satisfy */
    static size_t  sz     = 0;    /* number of stored terms       */
    static size_t  cap    = 0;    /* current capacity             */

    if (n == 0)                     /* undefined, just return 0    */
        return (seq_t)0;

    /* search until we have at least n terms ---------------------------- */
    while (sz < (size_t)n)
    {
        ++curr;                                /* candidate k = curr (1‑based) */

        seq_t prime  = (seq_t)get_prime(curr); /* pk  */
        seq_t sigma  = sigma_of(curr);       /* σ(k) */
        seq_t prod   = prime * sigma;        /* pk * σ(k) */

        /* 8*prod + 1 must be a perfect square of an odd integer   */
        seq_t tt   = prod * 8 + 1;
        seq_t ss   = floor_sqrt(tt);
        if (ss * ss == tt && (ss & 1))
        {
            /* grow storage if needed */
            if (sz == cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp;

#ifdef __cplusplus
                tmp = mem_resize(seq_t, found, new_cap);
#else
                tmp = realloc(found, new_cap * sizeof(seq_t));
#endif
                if (tmp == NULL)
                    exit(EXIT_FAILURE);
                found = tmp;
                cap   = new_cap;
            }
            found[sz++] = curr;                /* store k */
        }
    }

    return found[n-1];
}
