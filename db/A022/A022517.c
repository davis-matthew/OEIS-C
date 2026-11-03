/*
 * A022517  Describe previous term from the right (method B - initial term is
 *          6).
 * https://oeis.org/A022517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 6,
   a(n) = describe(a(n-1)),
   where describe reads the previous term from the right, groups identical
   consecutive digits, and for each group writes the digit followed by the
   count of that digit. */
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: produce the next term of the sequence by the description rule. */
static seq_t describe(seq_t x)
{
    seq_t result = (seq_t)0;

    while (x > (seq_t)0) {
        /* Current rightmost digit of the group. */
        seq_t digit = x % (seq_t)10;

        /* Count how many identical digits are in this group. */
        seq_t count = (seq_t)0;
        while (x > (seq_t)0) {
            seq_t cur = x % (seq_t)10;
            if (cur != digit)
                break;
            ++count;
            x /= (seq_t)10;
        }

        /* Compute 10^len(count), where len(count) is the number of decimal
           digits in 'count'. */
        seq_t tmp = count;
        unsigned long long lenCount = 0;
        while (tmp > (seq_t)0) {
            ++lenCount;
            tmp /= (seq_t)10;
        }
        seq_t pow10 = (seq_t)1;
        for (unsigned long long i = 0; i < lenCount; ++i)
            pow10 *= (seq_t)10;

        /* encode = digit followed by count (e.g., digit=1,count=12 → 112). */
        seq_t encode = digit * pow10 + count;
        seq_t pow10Encode = pow10 * (seq_t)10;

        /* Append encode to the right of the result. */
        result = result * pow10Encode + encode;
    }

    return result;
}

/* Memoization cache for computed terms. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of terms stored */

/* Ensure cache has capacity for at least 'needed' elements. */
static void cache_grow(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;

    /* In C++, cast the void* result of realloc to seq_t*. */
    cache = mem_resize(seq_t, cache, newcap);
    cache_cap = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is zero‑based; we need terms 0..n inclusive. */
    size_t need = (size_t)n + 1;
    cache_grow(need);

    while (cache_len <= n) {
        if (cache_len == 0) {
            cache[0] = (seq_t)6;
        } else {
            cache[cache_len] = describe(cache[cache_len - 1]);
        }
        ++cache_len;
    }

    return cache[(size_t)n];
}
