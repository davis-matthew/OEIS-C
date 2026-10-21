/*
 * A056558  Third tetrahedral coordinate, i.e., tetrahedron with T(t,n,k)=k;
 *          succession of growing finite triangles with increasing values
 *          towards bottom right.
 * https://oeis.org/A056558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A056558 – Third tetrahedral coordinate
 *  Formula: a(n) is the third coordinate k in the unique decomposition
 *           n = C(i,3) + C(j,2) + k with i > j > k ≥ 0
 *  The function returns k as a seq_t value. */

#include <stdint.h>

/* 64‑bit binomial coefficients (safe with 128‑bit intermediate) */
static inline idx_t C2(idx_t n) {
    return (idx_t)(((unsigned __int128)n * (n - 1)) / 2);
}

static inline idx_t C3(idx_t n) {
    return (idx_t)(((unsigned __int128)n * (n - 1) * (n - 2)) / 6);
}

/* binary search for the largest x such that Ck(x) <= value */
static idx_t max_with_C3(idx_t value) {
    idx_t low = 0;
    idx_t high = 4000000ULL; /* sufficient for n <= 2^64 - 1 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (C3(mid) <= value)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static idx_t max_with_C2(idx_t value, idx_t upper_bound) {
    idx_t low = 0;
    idx_t high = upper_bound;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (C2(mid) <= value)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find i: C(i,3) <= n < C(i+1,3) */
    idx_t i = max_with_C3(n);

    /* Remaining part after subtracting the tetrahedral contribution */
    idx_t rem = n - C3(i);

    /* Find j: C(j,2) <= rem < C(j+1,2), with j < i */
    idx_t j = max_with_C2(rem, i - 1);

    /* k is the remaining part */
    idx_t k = rem - C2(j);

    return (seq_t)k;
}
