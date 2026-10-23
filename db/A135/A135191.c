/*
 * A135191  Numbers n that raised to the powers from 1 to k (with k>=1) are
 *          multiples of the sum of their digits (n raised to k+1 must not be
 *          a multiple). Case k=6.
 * https://oeis.org/A135191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A135191: numbers n such that n^k is divisible by the sum of
 * the decimal digits of n^k for k = 1,…,6, and n^7 is NOT divisible by
 * the sum of the digits of n^7.  The implementation searches for
 * valid candidates in ascending order and stores the results in a
 * static cache so that multiple calls with arbitrary order are
 * efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: sum of decimal digits of a 128‑bit unsigned integer.        */
/* ------------------------------------------------------------------ */
static uint64_t digit_sum128(unsigned __int128 v)
{
    uint64_t sum = 0;
    while (v > 0) {
        sum += (uint64_t)(v % 10);
        v /= 10;
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Test whether the candidate m satisfies the divisibility
 * conditions.  m is assumed to fit in a 64‑bit unsigned integer.     */
/* ------------------------------------------------------------------ */
static bool valid_candidate(uint64_t m)
{
    unsigned __int128 power = m;      /* m^1 */
    for (int k = 1; k <= 6; ++k) {
        uint64_t s = digit_sum128(power);
        if (s == 0 || (power % s) != 0)
            return false;
        power *= m;                   /* advance to next power */
    }
    /* Now power == m^7 */
    uint64_t s7 = digit_sum128(power);
    if (s7 == 0)
        return false;
    return (power % s7) != 0;
}

/* ------------------------------------------------------------------ */
/* cache for already found terms                                      */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;   /* next integer to test */

/* Append a new term to the cache, growing it if necessary */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Public function: returns the n-th term of the sequence              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Find the next valid candidate */
        while (!valid_candidate(next_candidate))
            ++next_candidate;
        append_cache((seq_t)next_candidate);
        ++next_candidate;
    }
    return cache[n - 1];
}
