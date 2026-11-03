/*
 * A087657  a(n) = abs(prime(n)-a(n-1)) + abs(prime(n)-a(n-2)) +
 *          abs(prime(n)-a(n-3)).
 * https://oeis.org/A087657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs(prime(n)-a(n-1)) + abs(prime(n)-a(n-2)) +
 *            abs(prime(n)-a(n-3))
 * Range:     u8 n <= 54, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Offset: 1 (a(1) = 1, a(2) = 1, a(3) = 12) */

#include <stddef.h>                       /* for size_t */
#include <stdint.h>
#include <stdlib.h>
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *a_cache = NULL;   /* cache for a(1..k) values (index 0 unused) */
static size_t known = 0;        /* largest index computed so far (>= 0) */
static size_t cap = 0;          /* current allocated capacity of a_cache */

seq_t a(idx_t n) {
    /* Undefined behaviour for n==0; return 0 to avoid accessing cache[-1] */
    if (n == 0) return 0;

    /* If already computed, return the cached value */
    if ((size_t)n <= known) return a_cache[(size_t)n];

    /* Expand cache to hold up to n (index 0 unused) */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        grow_seq(&a_cache, &cap, need);
    }

    /* Compute new elements from known+1 up to n */
    for (size_t i = known + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            a_cache[i] = 1;
        } else if (i == 2) {
            a_cache[i] = 1;
        } else if (i == 3) {
            a_cache[i] = 12;
        } else {
            /* Prime at position i */
            uint32_t p_u32 = get_prime((idx_t)i);
            seq_t p = (seq_t)p_u32;

            /* Previous three terms */
            seq_t a1 = a_cache[i - 1];
            seq_t a2 = a_cache[i - 2];
            seq_t a3 = a_cache[i - 3];

            /* Absolute differences (prevent unsigned underflow) */
            seq_t d1 = (p > a1) ? (p - a1) : (a1 - p);
            seq_t d2 = (p > a2) ? (p - a2) : (a2 - p);
            seq_t d3 = (p > a3) ? (p - a3) : (a3 - p);

            a_cache[i] = d1 + d2 + d3;
        }
    }

    known = (size_t)n;
    return a_cache[(size_t)n];
}
