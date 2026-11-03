/*
 * A090431  Difference between sums of digits of n and n-th prime.
 * https://oeis.org/A090431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = digit_sum(n) - digit_sum(p_n)
 * Range:     i8 n <= 31, i16 n <= 3566, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9099 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_n is the n-th prime (p_1 = 2). */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

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

/*
 * Compute the difference between the sum of decimal digits of n
 * and the sum of decimal digits of the n-th prime.
 *
 * The function is compatible with the generic build environment
 * for seq_t and idx_t.  It simply casts the idx_t values to seq_t
 * before performing digit_sum, which works for any integer type
 * of seq_t.
 */
seq_t a(idx_t n)
{
    seq_t sum_n = digit_sum((seq_t)n);
    seq_t sum_p = digit_sum((seq_t)get_prime(n));
    return sum_n - sum_p;
}
