/*
 * A204127  Symmetric matrix based on f(i,j)=(F(i+1) if i=j and 1 otherwise),
 *          where F=A000045 (Fibonacci numbers), by antidiagonals.
 * https://oeis.org/A204127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = f(i,j) where the symmetric matrix M is M(i,j) =
 *            Fibonacci(i+1) if i=j, otherwise 1. The matrix is read by
 *            successive antidiagonals, starting with (1,1). The n‑th entry in
 *            this ordering is a(n)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

/* Integer square root for 64‑bit unsigned integers.
   Returns ⌊√x⌋.  Binary search is used to avoid floating point. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid <= n / mid) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined for n<1, harmless */

    /* Determine which antidiagonal contains the n-th element.
       The cumulative size up to antidiagonal D is T(D) = (D-1)*D/2. */
    uint64_t x = 8 * n + 1;                    /* 8n+1 fits in 64 bits for n≈10⁴ */
    uint64_t s = isqrt_u64(x);                 /* floor sqrt(8n+1) */
    uint64_t D;
    if (s * s == x) {                         /* perfect square */
        D = (s + 1) / 2;
    } else {
        D = (s + 1) / 2 + 1;
    }

    uint64_t prev   = (D - 2) * (D - 1) / 2;   /* entries before this diagonal */
    uint64_t offset = n - prev - 1;            /* 0‑based offset within diagonal */
    uint64_t i = offset + 1;                   /* row index (1‑based) */
    uint64_t j = D - i;                        /* column index */

    if (i == j)                                /* diagonal entry */
        return fib(i + 1);                     /* Fibonacci(i+1) */
    else
        return (seq_t)1;                       /* off‑diagonal entry */
}
