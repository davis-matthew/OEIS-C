/*
 * A057049  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057049(n) = i(n^2).
 * https://oeis.org/A057049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i(n^2) where (i(k),j(k)) is the position of k in an
 *            infinite grid enumerated along antidiagonals 1; 2,3; 4,5,6; ...
 *            . For k = n^2 let d be the smallest integer with d(d+1)/2 ≥ k.
 *            Then i(k) = k – (d‑1)d/2. This function computes i(n^2) with
 *            64‑bit arithmetic and 128‑bit intermediate product...
 * Range:     u8 n <= 198, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer (floor). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32; /* sqrt(2^64‑1) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__uint128_t)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t k = n * n;                     /* k = n²  (fits in idx_t for all test n) */
    idx_t tmp = 8 * k + 1;               /* 8k+1 – fits in idx_t for the test range */
    idx_t s   = isqrt_u64(tmp);          /* floor(sqrt(8k+1)) */
    idx_t d   = (s - 1) / 2;             /* floor((sqrt-1)/2) */

    /* Ensure d is the smallest integer with d(d+1)/2 ≥ k */
    idx_t total = d * (d + 1) / 2;
    if (total < k) {
        ++d;
        total = d * (d + 1) / 2;
    }

    idx_t tprev = (d - 1) * d / 2;        /* sum of previous diagonals */
    idx_t i     = k - tprev;              /* row index of k in the antidiagonal */

    return (seq_t)i;                      /* cast to the sequence element type */
}
