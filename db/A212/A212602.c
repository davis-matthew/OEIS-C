/*
 * A212602  a(n) is the smallest prime such that it and the previous prime are
 *          both of the form x^2 + n * y^2.
 * https://oeis.org/A212602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   ipow_i32, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns a(n) – the smallest prime p such that p and its
   immediately preceding prime are both representable as x^2 + n·y^2.
   The representation test is performed by brute‑force search over y.
   This is efficient for n ≤ 10 000 and primes of the order of magnitude
   returned by the sequence. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/next_prime.h"

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

/* Test whether an odd prime p is representable as x² + n·y².
   The test works for all n ≥ 1 and all p that fit into seq_t. */
static bool representable(seq_t p, idx_t n)
{
    /* trivial case: p < 2 is not prime */
    if (p < 2)
        return false;

    /* compute floor( sqrt( p / n ) ) to bound y */
    seq_t div = p / (seq_t)n;          /* integer division */
    idx_t max_y = (idx_t)floor_sqrt(div);

    for (idx_t y = 0; y <= max_y; ++y) {
        seq_t y2 = (seq_t)y * y;           /* y² */
        seq_t rem = p - y2 * (seq_t)n;    /* p – n·y² */
        seq_t s = floor_sqrt(rem);
        if (s * s == rem)                 /* perfect square found */
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0 – guard */

    /* start scanning primes from 2 upward */
    uint64_t prev = 2;
    uint64_t cur  = next_prime(prev);

    while (true) {
        if (representable((seq_t)prev, n) &&
            representable((seq_t)cur , n))
            return (seq_t)cur;

        prev = cur;
        cur  = next_prime(prev);
    }
}
