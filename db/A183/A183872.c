/*
 * A183872  n+floor(sqrt(5n)); complement of A183873.
 * https://oeis.org/A183872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(5*n))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(5*n)) for 0 <= n */
static inline seq_t floor_sqrt5(idx_t n)
{
    /* 5*n fits in 69 bits; use 128‑bit for the comparison */
    unsigned __int128 rhs = (unsigned __int128)n * 5;

    /* An upper bound for sqrt(5*n) for any 64‑bit n is 2^34‑1. */
    idx_t lo = 0;
    idx_t hi = (1ULL << 34) - 1;
    idx_t best = 0;

    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;

        if (mid2 <= rhs) {
            best = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    return (seq_t)best;
}

seq_t a(idx_t n)
{
    seq_t n_seq   = (seq_t)n;
    seq_t sqrt5n  = floor_sqrt5(n);
    return n_seq + sqrt5n;
}
