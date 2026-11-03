/*
 * A125256  Smallest odd prime divisor of n^2 + 1.
 * https://oeis.org/A125256
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 68, u32 n <= 20001, u64 n <= 20001,
 *            u128 n <= 20001, big any n
 * Validated: standard, pass, 83 terms through n = 9301 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes:  a(n) = smallest odd prime divisor of n^2 + 1
   (n ≥ 2).  For odd n the factor 2 is removed first, otherwise
   n^2+1 is already odd.  The search terminates at the integer
   square root of the odd part.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

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

seq_t a(idx_t n)
{
    if (n < 2)               /* offset is 2; behavior below it is irrelevant */
        return 0;

    /* n*n + 1 fits into seq_t for all n that the tests use */
    seq_t N = (seq_t)n * (seq_t)n + 1;

    if (n & 1) {             /* n odd → N even, remove the factor 2 */
        seq_t m = N / 2;      /* m = (n^2 + 1) / 2  */
        seq_t limit = floor_sqrt(m);
        seq_t p = 3;
        for (; p <= limit; p += 2)
            if (m % p == 0)
                return (seq_t)p;     /* smallest odd prime divisor found */

        /* m is prime itself (otherwise we would have succeeded above) */
        return m;
    }
    else {                   /* n even → N is odd, search directly */
        seq_t limit = floor_sqrt(N);
        seq_t p = 3;
        for (; p <= limit; p += 2)
            if (N % p == 0)
                return (seq_t)p;     /* smallest odd prime divisor found */

        /* N is prime itself */
        return N;
    }
}
