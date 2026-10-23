/*
 * A185543  Numbers not of the form floor(k^(3/2)); complement of A000093.
 * https://oeis.org/A185543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 215, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A185543: Numbers not of the form floor(k^(3/2)).
 * a(n) is the nth positive integer that is NOT equal to floor(k^(3/2))
 * for any integer k ≥ 1. The function searches candidate integers
 * sequentially, skipping those that occur as floor(k^(3/2)),
 * and returns the n-th remaining integer.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned integer.
   Returns floor(sqrt(n)). */
static uint64_t isqrt64(uint64_t n) {
    uint64_t low = 0, high = n >> 1 | 1;   /* high = max(n/2,1) */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Return floor(k^(3/2)) as a 64‑bit unsigned integer.
   k^3 fits in 64 bits for the ranges we need (k ≤ ~20 000). */
static uint64_t floor_k_cubic_half(uint64_t k) {
    uint64_t k3 = (unsigned __int128)k * k * k;  /* safe intermediate */
    return isqrt64(k3);
}

/* External linkage required by the build system. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* offset 1: undefined for n==0 */

    uint64_t target = (uint64_t)n;   /* we need the n‑th term */

    uint64_t count = 0;   /* number of valid terms found so far */
    uint64_t x = 1;       /* current candidate integer */
    uint64_t k = 1;       /* current k for floor(k^(3/2)) */
    uint64_t repr = floor_k_cubic_half(k);

    while (count < target) {
        /* Advance k until its value catches up to or surpasses x. */
        while (repr < x) {
            ++k;
            repr = floor_k_cubic_half(k);
        }

        /* If x is not one of the floor(k^(3/2)) values, count it. */
        if (repr != x) {
            ++count;
            if (count == target)
                return (seq_t)x;
        }
        ++x;  /* move to next candidate */
    }

    /* Should never reach here. */
    return (seq_t)x;
}
