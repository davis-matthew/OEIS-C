/*
 * A189409  a(n) = prime(n)#^2 + 1, where prime(n)# is the n-th primorial
 *          (A002110).
 * https://oeis.org/A189409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)#^2 + 1, where prime(n)# = product of the first n
 *            primes (with prime(0)# = 1). Off‑by‑zero indexing is used: a(0)
 *            = 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 162 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* Cache for primorials: P[0] = 1, P[i] = product of first i primes. */
    static seq_t *P = NULL;          /* array of primorials */
    static size_t capacity = 0;      /* current array capacity */
    static idx_t max_cached = 0;     /* highest i for which P[i] is computed */

    if (n == 0)            /* a(0) = 1^2 + 1 = 2 */
        return (seq_t)2;

    /* Ensure array capacity at least n+1 */
    if (capacity < (size_t)(n + 1)) {
        grow_seq_array(&P, &capacity, (size_t)(n + 1));
        if (max_cached == 0)                /* first allocation */
            P[0] = (seq_t)1;
    }

    /* Compute any missing primorials up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t prime_val = (seq_t)get_prime(i);           /* i is 1‑based */
        P[i] = seq_mul(P[i - 1], prime_val);
    }
    max_cached = n;

    /* a(n) = P[n]^2 + 1 */
    seq_t res = seq_mul(P[n], P[n]);
    res += (seq_t)1;
    return res;
}
