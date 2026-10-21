/*
 * A094436  Triangular array T(n,k) = Fibonacci(k+1)*binomial(n,k) for k =
 *          0..n; n >= 0.
 * https://oeis.org/A094436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Fibonacci(k+1) * Binomial(r, k) where r and k satisfy
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n – r*(r+1)/2. This
 *            produces the row‑major flattening of the triangular array T(r,
 *            k) = F(k+1)·C(r, k)
 * Range:     u8 n <= 39, u16 n <= 111, u32 n <= 364, u64 n <= 1252,
 *            u128 n <= 4709, big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative index up to r is ≤ n */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    idx_t r = high;                     /* largest r with r*(r+1)/2 ≤ n */
    idx_t k = n - r * (r + 1) / 2;       /* column index within row r */

    seq_t bin = binomial(r, k);          /* Binomial coefficient C(r, k) */
    seq_t f   = fib(k + 1);              /* Fibonacci number F(k+1) (F(0)=0, F(1)=1) */

    return bin * f;                      /* T(r, k) */
}
