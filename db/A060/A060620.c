/*
 * A060620  Average of the first n primes rounded down.
 * https://oeis.org/A060620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (p1 + p2 + … + pn) / n ) where pk is the k‑th
 *            prime
 * Range:     u8 n <= 13, u16 n <= 199, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 852 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the average of the first n primes, rounded down. */
#include <stdlib.h>
#include <stddef.h>
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

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* cumulative sums: cum_sum[i] = sum of first i primes, cum_sum[0] = 0 */
    static seq_t *cum_sum = NULL;
    static size_t capacity = 0;    /* allocated size (in seq_t elements) */
    static idx_t computed_n = 0;   /* highest n with cum_sum[0..n] computed */

    /* Do nothing for n == 0; result is undefined but harmless. */
    if (n == 0) return (seq_t)0;

    /* Expand storage if needed and compute missing terms. */
    if (n > computed_n) {
        /* Ensure the array can hold indices 0..n. */
        if (capacity < (size_t)(n + 1)) {
            size_t new_cap = capacity ? capacity * 2 : 64;
            while (new_cap < (size_t)(n + 1))
                new_cap *= 2;
            /* cast is required for C++ compilation */
            seq_t *tmp = mem_resize(seq_t, cum_sum, new_cap);
            if (!tmp) abort();   /* allocation failure should never happen */
            cum_sum = tmp;
            capacity = new_cap;
        }

        /* Base case: sum of 0 primes is 0. */
        if (computed_n == 0)
            cum_sum[0] = (seq_t)0;

        /* Compute primes and cumulative sums up to n. */
        for (idx_t i = computed_n + 1; i <= n; ++i) {
            seq_t prime = get_prime(i);          /* prime number */
            cum_sum[i] = cum_sum[i - 1] + prime; /* cumulative sum */
        }
        computed_n = n;
    }

    /* Integer division truncates toward 0, which is floor for positive values. */
    return cum_sum[n] / (seq_t)n;
}
