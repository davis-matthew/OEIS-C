/*
 * A236223  Sum of the fifteenth powers of the first n primes.
 * https://oeis.org/A236223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} prime(k)^15
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 8, u128 n <= 68,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 975 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow.h"
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
    /* static cache of prefix sums: pref[0] = 0,
       pref[i] = sum_{k=1..i} prime(k)^15  (i>=1)   */
    static seq_t *pref = NULL;
    static idx_t pref_len = 0;   /* last computed index */
    static idx_t pref_cap = 0;   /* allocated capacity (number of elements) */

    /* Initial allocation */
    if (!pref)
    {
        pref_cap = (n > 16 ? n + 4 : 17);   /* at least 17 elements */
        pref = mem_new(seq_t, pref_cap);
        if (!pref) return 0;                 /* out of memory */
        pref[0] = 0;
        pref_len = 0;
    }

    /* Grow capacity if needed (ensure index n is valid) */
    if (n >= pref_cap)
    {
        idx_t new_cap = n + 4;
        seq_t *tmp = mem_resize(seq_t, pref, new_cap);
        if (!tmp) return 0;
        pref = tmp;
        pref_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = pref_len + 1; i <= n; ++i)
    {
        uint32_t p = get_prime(i);                /* prime(i) */
        seq_t term = ipow((seq_t)p, (seq_t)15);/* p^15 */
        pref[i] = pref[i-1] + term;               /* prefix sum */
    }
    pref_len = n;

    return pref[n];
}
