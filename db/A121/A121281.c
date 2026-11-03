/*
 * A121281  Triangle T(n,k) read by rows: T(n,0) = A002110(n) and T(n,k) =
 *          A002110(n)/prime(k) for 1<=k<=n.
 * https://oeis.org/A121281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the unique non‑negative integer with * r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2 and set c = n – r*(r+1)/2. * Then a(n) =
 *            primorial(r) / ( c==0 ? 1 : prime(c) ), * where primorial(0)=1
 *            and prime(1)=2 is the first prime
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 54, u64 n <= 135,
 *            u128 n <= 377, big any n
 * Validated: standard, pass, 61 terms through n = 6787 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* GSL-type utilities for big integers and prime retrieval. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* Find row r and column c for index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t c = n - r * (r + 1) / 2;   /* 0 ≤ c ≤ r */

    /* Lazy creation and expansion of primorial cache. */
    static seq_t *primorial = NULL;
    static idx_t cached_r = 0;          /* highest computed primorial index */

    /* Ensure cache contains at least primorial[0]. */
    if (!primorial) {
        primorial = mem_new(seq_t, 1);
        if (!primorial) return 0;            /* allocation error → zero */
        primorial[0] = (seq_t)1;             /* primorial(0) = 1 */
        cached_r = 0;
    }

    /* Expand cache if necessary. */
    if (r > cached_r) {
        seq_t *tmp = mem_resize(seq_t, primorial, (r + 1));
        if (!tmp) return 0;                  /* allocation error → zero */
        primorial = tmp;
        /* Compute primorials from cached_r+1 up to r.  */
        for (idx_t i = cached_r + 1; i <= r; ++i) {
            primorial[i] = primorial[i - 1] *
                           (seq_t)get_prime(i); /* prime(i) for i ≥ 1 */
        }
        cached_r = r;
    }

    seq_t val = primorial[r];
    if (c == 0)
        return val;                                 /* first element of row */
    else
        return val / (seq_t)get_prime(c);           /* divide by prime(c) */
}
