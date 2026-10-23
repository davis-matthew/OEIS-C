/*
 * A160947  Numbers k that are multiples of the digital sum of k+1.
 * https://oeis.org/A160947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer k with k % ds(k+1) == 0 where
 *            ds(x) is the sum of the decimal digits of x
 * Range:     u8 n <= 24, u16 n <= 1900, u32 n <= 1900, u64 n <= 1900,
 *            u128 n <= 1900, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"   /* digit_sum(seq_t) */

#ifdef __cplusplus
#include <vector>
#endif

/* Return the n-th term of A160947, 1‑based index. */
seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------------- */
#ifdef __cplusplus
    /* C++ path: use std::vector, which handles arbitrary‑precision types.  */
    static std::vector<seq_t> cache;
    static seq_t next = 1;               /* next integer to test            */
#else
    /* C path: use malloc/realloc (seq_t must be trivially copyable).          */
    static seq_t *cache = NULL;          /* array of found terms             */
    static size_t cache_cap = 0;         /* capacity of the array            */
    static size_t cache_len = 0;         /* number of terms stored           */
    static seq_t next = 1;               /* next integer to test             */
#endif

    /* enlarge cache / vector if needed --------------------------------------- */
#ifdef __cplusplus
    if (cache.size() < (size_t)n) {
        size_t new_len = cache.size() ? cache.size() * 2 : 8;
        cache.reserve(new_len);
    }
#else
    while (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory – terminate       */
        cache = tmp;
        cache_cap = new_cap;
    }
#endif

    /* generate terms until we have at least n of them ------------------------- */
    while (
#ifndef __cplusplus
           cache_len < (size_t)n
#else
           cache.size() < (size_t)n
#endif
          )
    {
        seq_t ds = digit_sum(next + (seq_t)1);      /* ds(k+1) for k = next */
        if (ds != (seq_t)0 && (next % ds) == (seq_t)0) {
#ifdef __cplusplus
            cache.push_back(next);
#else
            cache[cache_len++] = next;
#endif
        }
        ++next;
    }

    /* fetch the requested term ---------------------------------------------- */
#ifdef __cplusplus
    return cache[(size_t)n - 1];
#else
    return cache[(size_t)n - 1];
#endif
}
