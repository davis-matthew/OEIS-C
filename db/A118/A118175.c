/*
 * A118175  Binary representation of n-th iteration of the Rule 220 elementary
 *          cellular automaton starting with a single black cell.
 * https://oeis.org/A118175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = round(sqrt(n+1)) - floor(sqrt(n))
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              a(n) = 1 - A079813(n+1)                                  */
/* This implementation uses only integer arithmetic, no floating point.   */
/* It works for any idx_t value (uint64_t) and any seq_t (8‑128 bit).      */

#include <stdint.h>   /* for uint64_t used in local calculations */

/* Integer floor square root for 64‑bit values.
   Returns the largest x such that x*x <= n. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    /* Squares of numbers greater than (1ULL<<32)-1 would overflow uint64_t. */
    if (n < 2ULL) return n;            /* 0 and 1 are special cases */

    idx_t lo = 0;
    idx_t hi = (1ULL << 32) - 1;             /* because (2^32-1)^2 < 2^64 */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;                 /* fits in uint64_t */

        if (sq == n) return mid;
        if (sq <  n)  lo = mid + 1;
        else          hi = mid - 1;
    }
    return lo - 1;      /* lo is the first value with sq > n */
}

/* Rounded square root of a non‑negative integer:
   returns floor sqrt(x) or floor sqrt(x) + 1, whichever is closest.
   Ties (when fractional part == 0.5) round up. */
static inline idx_t round_sqrt_u64(idx_t x)
{
    idx_t r = floor_sqrt_u64(x);            /* floor sqrt(x) */

    /* Use 128‑bit arithmetic to avoid overflow when testing (2*r+1)^2 */
    unsigned __int128 left  = (unsigned __int128)4 * (unsigned __int128)x;
    unsigned __int128 t     = (unsigned __int128)2 * (unsigned __int128)r + 1;
    unsigned __int128 right = t * t;

    /* If 4*x >= (2*floor√x + 1)^2 then sqrt(x) >= floor+0.5 => round up */
    return (left >= right) ? r + 1 : r;
}

seq_t a(idx_t n)
{
    /* floor(sqrt(n)) */
    idx_t fs_n  = floor_sqrt_u64(n);

    /* round(sqrt(n+1)) */
    idx_t rs_np1 = round_sqrt_u64(n + 1);

    /* a(n) = round(sqrt(n+1)) - floor(sqrt(n)) */
    return (seq_t)(rs_np1 - fs_n);
}
