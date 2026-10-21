/*
 * A097291  Contains exactly once every pair (i,j) of positive integers.
 * https://oeis.org/A097291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097291: enumeration of all (i,j) pairs.
   For index n (n ≥ 1) find the segment j = ceil(sqrt(n)).
   The segment consists of 1, j, j, (2,j), (3,j), … , (j‑1,j).
   The function computes the term without any table lookup. */

#include <stdint.h>

/* Integer square‑root: returns floor(sqrt(n)) for unsigned 64‑bit n. */
static inline idx_t isqrt(idx_t n)
{
    /* Binary search on [0, 4294967296) – the largest integer whose square
       fits in 64 bits is 4294967295. */
    idx_t low = 0, high = 4294967296ULL;      /* one more than max sqrt(2^64-1) */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        /* Use division to avoid overflow in mid*mid. */
        if (mid == 0 || mid <= n / mid)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find the segment number j = ceil(sqrt(n)). */
    idx_t j = isqrt(n);
    if (j * j < n)
        ++j;                     /* make it the ceiling */

    /* Offset of n within its segment: 0‑based. */
    idx_t prev_sq = (j - 1) * (j - 1);
    idx_t offset = n - prev_sq - 1;   /* 0 … 2j‑2 */

    if (offset == 0)      /* the leading 1 */
        return (seq_t)1;

    if (offset == 1 || offset == 2)    /* the two j’s after the 1 */
        return (seq_t)j;

    /* Remaining positions encode (i,j) pairs. */
    idx_t m = offset - 3;                 /* 0 … 2(j‑2)-1 */
    if ((m & 1) == 0)                     /* even m → return i */
        return (seq_t)(m / 2 + 2);
    else                                   /* odd m  → return j */
        return (seq_t)j;
}
