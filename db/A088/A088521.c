/*
 * A088521  a(1) = 1; for n > 1, a(n) = (a(n-1) + n) mod prime(n).
 * https://oeis.org/A088521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = (a(n-1) + n) mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 958 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(n) is the n‑th prime (prime(1)=2, prime(2)=3, …). */

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

static seq_t *a_arr    = NULL;   /* 1‑indexed array of computed values */
static idx_t  a_size   = 0;      /* currently allocated size (>=1) */
static idx_t  a_max    = 0;      /* largest n for which a_arr[n] is valid */

/* Ensure a_arr is large enough to hold indices up to n (1‑indexed). */
static void extend_a_arr(idx_t n)
{
    if (a_size >= n + 1)
        return;
    size_t new_size = (n + 1) * 2;
    seq_t *tmp = mem_resize(seq_t, a_arr, new_size);
    if (!tmp)
        return; /* allocation failure – caller will get an incorrect result */
    a_arr = tmp;
    a_size = (idx_t)new_size;
}

/* Return the n‑th prime as a seq_t.  The helper get_prime returns uint32_t,
   which is safe for n <= 10⁴ (prime(10000)=104729). */
static seq_t nth_prime(idx_t n)
{
    return (seq_t)get_prime(n);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined; handle gracefully */

    /* Extend storage for n. */
    extend_a_arr(n);

    /* Initialize the sequence: a(1) = 1. */
    if (a_max == 0) {
        a_arr[1] = (seq_t)1;
        a_max = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t k = a_max + 1; k <= n; ++k) {
        seq_t prev = a_arr[k - 1];
        seq_t add  = (seq_t)k;
        seq_t mod  = nth_prime(k);
        a_arr[k] = (prev + add) % mod;
    }
    a_max = (a_max > n) ? a_max : n;

    return a_arr[n];
}
