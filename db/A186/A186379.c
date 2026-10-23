/*
 * A186379  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=4i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186380.
 * https://oeis.org/A186379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((sqrt(32*n - 3) - 1)/2)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS sequence A186379 for all n >= 1. */
/* The function uses only integer arithmetic and no floating‑point. */

#include <stdint.h>

/* Compute floor(sqrt(x)) for an unsigned 128‑bit integer x.
   The result is guaranteed to fit in a 64‑bit unsigned integer. */
static uint64_t floor_sqrt_u128(unsigned __int128 x)
{
    if (x == 0ULL) return 0ULL;
    uint64_t lo = 0;
    uint64_t hi = UINT64_MAX;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;   /* hi is the floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety we handle n=0. */
    if (n == 0) return 0;

    /* Compute M = 32*n - 3 as a 128‑bit value. */
    unsigned __int128 M = (unsigned __int128)n * 32ULL - 3ULL;

    /* r = floor(sqrt(M)).  r is at most 2^64-1. */
    uint64_t r = floor_sqrt_u128(M);

    /* Compute t = floor((r - 1) / 2)   (equivalent to floor((sqrt(M)-1)/2)). */
    uint64_t t = (r / 2) - ((r & 1ULL) == 0 ? 1ULL : 0ULL);

    /* a(n) = n + t.  All arithmetic is performed in seq_t. */
    return (seq_t)n + (seq_t)t;
}
