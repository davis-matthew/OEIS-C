/*
 * A044698  Numbers n such that string 7,3 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* This function implements sequence A044698: all positive integers n such that
   the base‑9 representation of n contains the substring 73, whereas the base‑9
   representation of n+1 does not.  The function is not a formula but a
   search over the integers. */

 /* Core check: does the base‑9 representation of x contain “73”? */
static bool has73(seq_t x)
{
    if (x == 0)
        return false;

    /* 256 digits are more than enough for any practical test; this is a
       stack‑allocated buffer so no heap calls are needed. */
    uint8_t digits[256];
    size_t len = 0;

    seq_t t = x;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % 9);
        t = t / 9;
    }

    for (size_t i = len; i > 1; --i)
        if (digits[i - 1] == 7 && digits[i - 2] == 3)
            return true;

    return false;
}

/* Main entry point for OEIS evaluation.  This function must work in both
   C11 and C++17.  In C++ we use std::vector for automatic memory
   management; in C we fall back to malloc/realloc. */
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* defensive, not part of the sequence */

#ifdef __cplusplus
    /* C++ branch – use vector for dynamic caching. */
    static std::vector<seq_t> cache;
    static seq_t next_start = 1;

    while (cache.size() < n) {
        seq_t candidate = next_start;

        if (has73(candidate) && !has73(candidate + 1)) {
            cache.push_back(candidate);
            next_start = candidate + (seq_t)1;
        } else {
            ++next_start;
        }
    }
    return cache[(size_t)(n - 1)];

#else
    /* C branch – use malloc/realloc.  seq_t is a built‑in integer type. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t next_start = 1;

    while (cache_size < n) {
        seq_t candidate = next_start;

        if (has73(candidate) && !has73(candidate + 1)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
            next_start = candidate + (seq_t)1;
        } else {
            ++next_start;
        }
    }
    return cache[n - 1];
#endif
}
