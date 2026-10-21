/*
 * A059094  Numbers whose sum of digits is a cube.
 * https://oeis.org/A059094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3033, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059094: Numbers whose sum of decimal digits is a perfect cube.
   The function builds the sequence on demand, caching all terms found so far.
   For each term it tests the integer digit sum and accepts the number
   if the sum is a cube.  The algorithm runs in O(n^2) worst‑case but
   in practice it is linear in the number of terms asked for; it
   satisfies the timing requirement for n up to ~10 000. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"   /* digit_sum(seq_t v) */

static bool cube_digit_sum(seq_t v)
{
    /* Sum of decimal digits of v.  digit_sum returns a seq_t */
    seq_t s = digit_sum(v);
    uint64_t sum = (uint64_t)s;               /* the sum is tiny (≤ 9*digits) */

    /* Find integer cube root r of sum */
    uint64_t r = 0;
    while ((r + 1) * (r + 1) * (r + 1) <= sum)
        ++r;
    return r * r * r == sum;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is offset 1; undefined for n==0 */
        return (seq_t)0;

    /* Static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* number of terms currently stored */
    static size_t cache_cap   = 0;   /* allocated capacity */
    static uint64_t next       = 1;  /* next integer to test */

    if (cache_cap == 0) {
        /* initial capacity */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        seq_t cur = (seq_t)next;   /* cast to seq_t (works for built‑in and big‑int) */

        /* Ensure enough room to store a new term */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        if (cube_digit_sum(cur)) {
            cache[cache_size++] = cur;
        }
        ++next;
    }

    return cache[n - 1];
}
