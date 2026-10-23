/*
 * A131410  A127647 * A000012.
 * https://oeis.org/A131410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the Fibonacci number F(r), where r is the least integer
 *            satisfying r(r+1)/2 ≥ n. The sequence is formed by listing, for
 *            each positive integer r, r copies of F(r). Thus the cumulative
 *            number of terms up to row r is the r‑th triangular number
 * Range:     u8 n <= 91, u16 n <= 300, u32 n <= 1128, u64 n <= 4371,
 *            u128 n <= 7260, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

/* Integer square root for a 64 bit unsigned value, using binary search.
   It returns ⌊√n⌋. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1ULL << 32);   /* √(2^64-1) < 2^32 */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* mid*mid cannot overflow: mid ≤ 2^32‑1 */
        if (mid * mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Find the row r that contains the index n. */
    idx_t t   = 8 * n + 1;                 /* 8n+1 for quadratic formula */
    idx_t s   = isqrt_u64(t);              /* floor(sqrt(8n+1))          */
    idx_t r   = (s - 1) >> 1;              /* floor((sqrt-1)/2)          */
    /* Correct possible off‑by‑one due to flooring. */
    while ((r * (r + 1)) / 2 < n)
        r++;

    return fib(r);
}
