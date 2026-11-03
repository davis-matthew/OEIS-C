/*
 * A087851  a(n) = abs(a(n-1) - floor(n*phi)), where phi=(1+sqrt(5))/2, with
 *          a(1) = a(2) = 1.
 * https://oeis.org/A087851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9064 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  AEIS A087851
    a(1) = a(2) = 1
    a(n) = |a(n-1) - floor(n*φ)|   for n > 2
    φ = (1+√5)/2
    Recurrence:   a(n) = |a(n-1) - floor( (n + floor(√(5*n*n)) ) / 2 )|
*/

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

static seq_t *seq_cache = NULL;     /* cache[1..len] holds a(1)…a(len)   */
static idx_t seq_cache_len = 0;     /* number of terms cached            */

/* compute floor(n*φ) with integer arithmetic */
static seq_t floor_phi(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t w  = sn * sn;         /* n²                                         */
    seq_t p  = w * 5;           /* 5·n²                                       */
    seq_t k  = floor_sqrt(p);   /* ⌊√(5·n²)⌋                                   */
    return (sn + k) / 2;        /* ⌊(n + ⌊√(5·n²)⌋)/2⌋ = ⌊n·φ⌋                    */
}

seq_t a(idx_t n)
{
    if (n <= 2)               /* base values – no caching needed */
        return 1;

    /* initialise cache with a(1) and a(2) if necessary */
    if (seq_cache_len < 2) {
        seq_cache_len = 2;
        seq_cache = mem_resize(seq_t, seq_cache, 3); /* indices 0..2 */
        seq_cache[1] = 1;
        seq_cache[2] = 1;
    }

    if (n <= seq_cache_len)
        return seq_cache[n];

    /* extend cache to contain up to a(n) */
    seq_cache = mem_resize(seq_t, seq_cache, (n + 1));
    for (idx_t i = seq_cache_len + 1; i <= n; ++i) {
        seq_t prev = seq_cache[i - 1];
        seq_t phi_i = floor_phi(i);
        seq_t val = (prev >= phi_i) ? (prev - phi_i) : (phi_i - prev);
        seq_cache[i] = val;
    }

    seq_cache_len = n;
    return seq_cache[n];
}
