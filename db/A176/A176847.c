/*
 * A176847  The odd non-semiprime numbers.
 * https://oeis.org/A176847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A176847: a(n) = the nth odd integer that is NOT a semiprime (product of exactly two primes) */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Test whether an odd integer m (>1) is a semiprime (exactly two prime factors counted with multiplicity). */
static bool is_semiprime_uint64(uint64_t m)
{
    if (m < 4) return false;               // 1,2,3 are not semiprimes
    uint64_t cnt = 0;
    uint64_t d = 3;
    while (d * d <= m && cnt <= 2) {
        while (m % d == 0) {
            ++cnt;
            m /= d;
            if (cnt > 2) return false;    // more than two prime factors
        }
        d += 2;
    }
    if (m > 1) ++cnt;                      // remaining prime factor
    return cnt == 2;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n < 1 */

    /* Static cache for generated terms. */
    static seq_t *cache   = NULL;   /* array of terms */
    static size_t  cap    = 0;      /* current capacity of the array */
    static size_t  len    = 0;      /* number of terms stored */
    static uint64_t last  = 1;      /* last candidate tested (odd integer) */

    /* Allocate initial cache if needed. */
    if (!cache) {
        cap = 64;                                     /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        len = 0;
        last = 1;                                      /* first odd integer */
    }

    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        uint64_t cand = (len == 0) ? 1 : last + 2;     /* odd candidates: 1,3,5,... */
        if (len != 0) last = cand;

        /* Store non‑semiprime odd numbers. */
        if (!is_semiprime_uint64(cand)) {
            if (len == cap) {                         /* grow cache if full */
                size_t newcap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;            /* allocation failure */
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
