/*
 * A166550  Numbers n with property that n^2 and n-th prime have the same sum
 *          of digits.
 * https://oeis.org/A166550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 902 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166550: numbers n such that sumdigits(n²) == sumdigits(prime(n)) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

#define MAX_CACHE 200000   /* sufficient for n up to 10000 */

static seq_t  cache[MAX_CACHE];
static size_t cache_len = 0;    /* number of stored terms */
static idx_t  checked_up_to = 0;/* largest n that has been inspected */

/* Ensure that at least n terms are cached. */
static void ensure_terms(idx_t n)
{
    while (cache_len < n) {
        ++checked_up_to;                         /* examine next candidate */

        /* digit sum of the square */
        seq_t m_sq = (seq_t)checked_up_to * (seq_t)checked_up_to;
        seq_t d_sq = digit_sum(m_sq);

        /* digit sum of the m-th prime */
        seq_t prime  = (seq_t)get_prime(checked_up_to);
        seq_t d_prime = digit_sum(prime);

        if (d_sq == d_prime) {
            if (cache_len == MAX_CACHE) {
                /* capacity exceeded – should not happen for the target range */
                abort();
            }
            cache[cache_len++] = checked_up_to;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 as fallback */
    ensure_terms(n);
    return cache[n - 1];
}
