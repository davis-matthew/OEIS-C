/*
 * A026255  Beginning with the natural numbers, swap [ k*sqrt(3) ] and [
 *          (k/2)*(3 + sqrt(3)) ], for all k >= 1.
 * https://oeis.org/A026255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026255: a self‑inverse permutation defined by two Beatty
   sequences.
   Let A_k = floor(k*sqrt(3))  and  B_k = floor((3 + sqrt(3))*k/2).
   Then a(A_k) = B_k  and  a(B_k) = A_k.  This function returns a(n)
   for n >= 1.  The implementation uses binary search on k without
   floating point arithmetic, relying only on integer operations
   and the provided floor_sqrt routine. */

#include <stdint.h>
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

static inline seq_t compute_A_k(idx_t k)
{
    seq_t kk = (seq_t)k;
    return floor_sqrt((seq_t)3 * kk * kk);          /* floor(k*sqrt(3)) */
}

static inline seq_t compute_B_k(idx_t k)
{
    seq_t kk = (seq_t)k;
    seq_t A  = compute_A_k(k);
    return ((seq_t)3 * kk + A) / 2;                 /* floor((3+sqrt(3))*k/2) */
}

seq_t a(idx_t n)
{
    /* Respect the offset: a(1) is the first term.  Undefined for n<1. */
    if (n == 0) return 0;

    seq_t target = (seq_t)n;
    idx_t low, high, mid;
    seq_t val;

    /* Search in the A‑sequence first */
    low = 1;  high = n;
    while (low <= high) {
        mid = low + (high - low) / 2;
        val = compute_A_k(mid);
        if (val == target) return compute_B_k(mid);
        if (val < target)
            low = mid + 1;
        else
            high = mid - 1;
    }

    /* Not found in A; search in B */
    low = 1;  high = n;
    while (low <= high) {
        mid = low + (high - low) / 2;
        val = compute_B_k(mid);
        if (val == target) return compute_A_k(mid);
        if (val < target)
            low = mid + 1;
        else
            high = mid - 1;
    }

    /* Should never reach here for n >= 1 */
    return 0;
}
