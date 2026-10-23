/*
 * A161419  First differences of A161418.
 * https://oeis.org/A161419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A161419 – first differences of A161418
 *
 * A161418(n) is the number of unordered factorizations of n
 * (including the trivial factorization n itself).
 * This sequence gives a(n) = 6·(A161418(n) – 1) for n ≥ 1.
 *
 * The function computes the number of factorizations recursively
 * in a depth‑first manner.  All calculations are performed with
 * 64‑bit unsigned integers and only the final result is cast to
 * seq_t, so the implementation works for any build with
 * seq_t and idx_t defined as in the exercise.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) – works for n up to 2^64-1 */
static uint64_t _isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;        /* hi > sqrt(2^64-1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)          /* mid*mid > n */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Count unordered multiplicative partitions of rem with factors ≥ min_factor.
 * The count includes the trivial partition consisting only of rem itself. */
static seq_t _count_parts(uint64_t rem, uint64_t min_factor)
{
    uint64_t total = 1;                       /* trivial partition */

    uint64_t limit = _isqrt_u64(rem);
    for (uint64_t f = min_factor; f <= limit; ++f) {
        if (rem % f == 0) {
            uint64_t next = rem / f;
            total += (uint64_t)_count_parts(next, f);
        }
    }
    return (seq_t)total;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                      /* a(1) = 0, a(0) is irrelevant */

    seq_t cnt = _count_parts((uint64_t)n, 2);   /* includes trivial part */
    seq_t diff = 6 * (cnt - 1);
    return diff;
}
