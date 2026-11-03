/*
 * A062281  Smallest multiple of n-th prime with all even digits.
 * https://oeis.org/A062281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 867 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A062281: The smallest multiple of the n‑th prime whose decimal
   representation contains only even digits (0,2,4,6,8).  The sequence
   is computed lazily and memoised; each term is searched by
   multiplying the prime by even integers until all digits are even. */
#include <stdbool.h>
#include <stdlib.h>
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

/* Test whether all decimal digits of x are even. */
static inline bool all_even_digits(seq_t x)
{
    if (x == 0)           /* 0 is considered to have only even digits */
        return true;

    while (x != 0) {
        seq_t dig   = x % 10;     /* least significant decimal digit   */
        if ((dig % 2) != 0)       /* odd digit found                    */
            return false;
        x /= 10;                  /* move to next digit                  */
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Memoisation storage for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure the cache can hold the requested index */
    if (n > cache_len) {
        size_t req = (size_t)n;
        seq_t *tmp = mem_resize(seq_t, cache, req);
        if (!tmp) { mem_free(cache); exit(EXIT_FAILURE); }  /* allocation failed */
        cache = tmp;

        /* Compute missing terms */
        for (size_t i = cache_len; i < req; ++i) {
            /* n is 1‑based, get_prime is 1‑based */
            uint32_t p_int = (uint32_t)get_prime((idx_t)(i + 1));
            seq_t prime = (seq_t)p_int;
            seq_t result;

            if (prime == (seq_t)2) {
                /* The first prime 2 itself satisfies the condition */
                result = (seq_t)2;
            } else {
                seq_t k = (seq_t)2;       /* start with the smallest even multiplier */
                while (true) {
                    seq_t cand = prime * k;
                    if (all_even_digits(cand)) {
                        result = cand;
                        break;
                    }
                    k += (seq_t)2;        /* only even multipliers can give an even last digit */
                }
            }
            cache[i] = result;
        }
        cache_len = req;
    }
    return cache[(size_t)n - 1];
}
