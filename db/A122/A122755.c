/*
 * A122755  Let f(m) = 10 - last digit of prime(m). Sequence gives numbers n
 *          such that (1/n)*Sum_{ m <= n } f(m) is <= 5.
 * https://oeis.org/A122755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A122755: n such that (1/n)*∑_{m≤n}f(m) ≤ 5, 
   where f(m) = 10 – (last digit of the m‑th prime).
   The sequence is produced by scanning successive integers n,
   accumulating the sum of f values, and recording those n
   that satisfy the inequality. */

#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n) {
    /* Static state retained between calls */
    static idx_t computed_upto = 0;         /* last n processed          */
    static uint64_t sum_f = 0;              /* Accumulated f values      */
    static seq_t *qual = NULL;              /* Array of qualifying N's   */
    static size_t qual_cap = 0;             /* Capacity of qual array    */
    static size_t qual_len = 0;             /* Number of entries stored  */

    /* Lazily allocate qualifier array */
    if (qual_cap == 0) {
        qual_cap = 16;
        qual = mem_new(seq_t, qual_cap);
    }
    /* Grow array if necessary */
    if (qual_len + 1 > qual_cap) {
        size_t new_cap = qual_cap << 1;
        if (new_cap < qual_len + 1) new_cap = qual_len + 1;
        qual = mem_resize(seq_t, qual, new_cap);
        qual_cap = new_cap;
    }

    /* Extend computation until we have at least n qualifying terms */
    while (qual_len < n) {
        computed_upto += 1;
        uint32_t p = get_prime(computed_upto);   /* p is the computed_upto‑th prime */
        uint32_t d = p % 10;                     /* last decimal digit               */
        uint64_t f = 10ull - (uint64_t)d;        /* f(m) value                       */
        sum_f += f;

        /* Check whether the average of f up to computed_upto is ≤ 5   */
        if (sum_f <= 5ull * (uint64_t)computed_upto) {
            qual[qual_len++] = (seq_t)computed_upto; /* store qualifying N */
        }
    }

    return qual[n - 1];
}
