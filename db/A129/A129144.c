/*
 * A129144  a(1)=a(2)=1; for n>2: a(n) = (s = sum of all previous terms) minus
 *          (maximal square less than s).
 * https://oeis.org/A129144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=1; for n>2: a(n)=s-(floor(sqrt(s)))^2, where
 *            s=sum_{i=1}^{n-1} a(i)
 * Range:     u8 n <= 18, u16 n <= 173, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 98 terms through n = 1525 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the sequence in a dynamic array.
   It works for both integer seq_t types (C++ arbitrary‑precision
   classes or unsigned integer types) as well as for idx_t=uint64_t. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *cache      = NULL; /* memoised values */
static size_t stored    = 0;      /* number of terms currently stored */
static size_t capacity  = 0;      /* allocated size of cache   */
static seq_t cumulative = 0;      /* sum of all stored terms   */

seq_t a(idx_t n)
{
    /* n>=1 by horizon.  Handle the first two terms explicitly. */
    if (n <= 2) return (seq_t)1;

    /* Initialise cache with the first two terms if needed. */
    if (stored < 2) {
        size_t need = 2;
        if (capacity < need) {
            cache = mem_resize(seq_t, cache, need);
            capacity = need;
        }
        cache[0] = cache[1] = (seq_t)1;
        stored = 2;
        cumulative = cache[0] + cache[1];   /* = 2 */
    }

    /* Build terms up to n. */
    while (stored < (size_t)n) {
        /* s = sum of previous terms */
        seq_t s = cumulative;

        /* k = floor(sqrt(s)) */
        seq_t k = floor_sqrt(s);
        seq_t ksq = k * k;

        /* Determine the square to subtract.
           If s itself is a perfect square, use (k-1)^2. */
        seq_t sub = (ksq == s) ? (k - 1) * (k - 1) : ksq;

        /* Next term */
        seq_t next = s - sub;

        /* Ensure capacity */
        if (stored >= capacity) {
            size_t newcap = capacity ? capacity * 2 : 8;
            while (newcap <= stored) newcap <<= 1;
            cache = mem_resize(seq_t, cache, newcap);
            capacity = newcap;
        }

        /* Store the new term and update running sum. */
        cache[stored] = next;
        cumulative += next;
        ++stored;
    }

    return cache[(size_t)n - 1];
}
