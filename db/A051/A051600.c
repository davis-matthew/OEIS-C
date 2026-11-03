/*
 * A051600  Rows of triangle formed using Pascal's rule except begin n-th row
 *          with (n+1)st prime and end it with (n+2)nd prime.
 * https://oeis.org/A051600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051600: rows of Pascal‑like triangle.
   Row n (n≥1) has n+1 entries.  The leftmost entry is the 2n‑th prime,
   the rightmost entry is the (2n+1)‑st prime, and interior entries are
   the sums of the two entries above it.  The sequence is the row‑wise
   flattening of this triangle. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
    /* static storage for the flattened triangle */
    static seq_t *cache = NULL;   /* flattened array of a(k) */
    static size_t cache_len = 0;  /* number of elements stored */

    /* static storage for the previous row of the triangle */
    static seq_t *prev = NULL;
    static idx_t  prev_len = 0;   /* number of elements in prev */

    /* initialise on first call: row 0 */
    if (cache == NULL) {
        prev_len = 1;
        prev = mem_new(seq_t, prev_len);
        prev[0] = (seq_t)get_prime(1);          /* 2 */

        cache_len = 1;
        cache = mem_new(seq_t, cache_len);
        cache[0] = prev[0];
    }

    /* generate rows until we have at least n+1 elements in cache */
    while (cache_len <= (size_t)n) {
        /* length of the new row = prev_len + 1 */
        idx_t new_len = prev_len + 1;

        /* allocate temporary array for the new row */
        seq_t *curr = mem_new(seq_t, new_len);

        /* leftmost and rightmost primes for this row */
        idx_t left_idx  = 2 * prev_len;      /* prime(2*prev_len)   */
        idx_t right_idx = left_idx + 1;      /* prime(2*prev_len+1) */

        curr[0]           = (seq_t)get_prime(left_idx);
        curr[new_len-1]   = (seq_t)get_prime(right_idx);

        /* interior elements via Pascal's rule */
        for (idx_t k = 1; k + 1 < new_len; ++k) {
            curr[k] = prev[k-1] + prev[k];
        }

        /* append the new row to the cache */
        size_t new_cache_len = cache_len + new_len;
        cache = mem_resize(seq_t, cache, new_cache_len);
        for (idx_t i = 0; i < new_len; ++i) {
            cache[cache_len + i] = curr[i];
        }
        cache_len = new_cache_len;

        /* free the old row and update prev pointers */
        mem_free(prev);
        prev      = curr;
        prev_len  = new_len;
    }

    /* return the requested element */
    return cache[(size_t)n];
}
