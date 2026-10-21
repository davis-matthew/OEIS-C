/*
 * A081946  a(n) = Sum_{i=1..n} floor(r*floor(i/r)), where r=sqrt(2).
 * https://oeis.org/A081946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=1..n} ⌊ √2 · ⌊i/√2⌋ ⌋ (equivalent to the definition
 *            given in A081946)
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square‑root of a 64‑bit unsigned integer.
   Uses binary search on 128‑bit intermediate to avoid overflow. */
static inline uint64_t isqrt64(uint64_t n)
{
    uint64_t lo = 0, hi = n + 1;          /* hi is an exclusive upper bound */
    while (lo + 1 < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if ((__int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Computes a(n) as required.  idx_t is an unsigned 64‑bit integer,
   seq_t is an unsigned integer type that is guaranteed to be large enough
   for the result. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* k = ⌊ i / √2 ⌋ = ⌊ √( i² / 2 ) ⌋ */
        uint64_t i_sq = (uint64_t)i;
        __int128 prod = (__int128)i_sq * i_sq;
        uint64_t k = isqrt64((uint64_t)(prod / 2));

        /* term = ⌊ √2 · k ⌋ = ⌊ √( 2 k² ) ⌋ */
        __int128 prod2 = (__int128)2 * k * k;
        uint64_t term = isqrt64((uint64_t)prod2);

        sum += (seq_t)term;
    }
    return sum;
}
