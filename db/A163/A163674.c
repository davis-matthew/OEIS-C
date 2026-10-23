/*
 * A163674  Triangle T(n,m) = 2*m*n + m + n + 9 read by rows.
 * https://oeis.org/A163674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163674: Triangle T(n,m) = 2*m*n + m + n + 9 read by rows.
   a(n) is the n-th term of the flattened triangle, offset 1. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: sequence is defined for n ≥ 1 */

    /* Find the row r such that r(r‑1)/2 < n ≤ r(r+1)/2. */
    idx_t low = 1, high = n;   /* high is definitely ≥ r */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t total = (__uint128_t)mid * (mid + 1) / 2;
        if (total < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;

    /* Compute 1‑based column index c in that row. */
    __uint128_t start128 = (__uint128_t)(r - 1) * r / 2 + 1;
    idx_t start = (idx_t)start128;
    idx_t c = n - start + 1;

    /* Evaluate T(r,c) = 2*r*c + r + c + 9. */
    seq_t row = (seq_t)r;
    seq_t col = (seq_t)c;
    return 2 * row * col + row + col + 9;
}
