/*
 * A094435  Triangular array read by rows: T(n,k) = Fibonacci(k)*C(n,k), k =
 *          1...n; n>=1.
 * https://oeis.org/A094435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(k) * C(r, k) where r is the row containing the
 *            * n-th term (rows are numbered 1,2,3,…) and k = n - r(r-1)/2 is
 *            the * position within that row. Thus n runs through the
 *            triangular array * read by rows. (See A094435.)
 * Range:     u8 n <= 31, u16 n <= 99, u32 n <= 339, u64 n <= 1251,
 *            u128 n <= 4615, big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"      /* seq_t fib(idx_t n);           */
#include "../utils/binomial.h" /* seq_t binomial(idx_t n, idx_t k); */

static inline idx_t triangular_row(idx_t idx)
{
    /* Find the smallest r such that r*(r+1)/2 ≥ idx.
     * Uses binary search on the monotone function f(r)=r*(r+1)/2.
     * All arithmetic that may overflow 64‑bit is performed with
     * __int128, which is safe for idx up to 2^64‑1.               */
    idx_t low = 1, high = 1;
    /* Exponentially increase high until it is large enough.      */
    while (((__int128)high * (high + 1)) / 2 < idx) {
        high <<= 1;
        if (high == 0) {          /* reached maximum representable value */
            high = (idx + 1) / 2; /* a safe upper bound for large idx   */
            break;
        }
    }
    /* Binary search between low and high.                        */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sum_mid = (__int128)mid * (mid + 1) / 2;
        if (sum_mid >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n is the 1‑based index in the triangular array read by rows.  */
    idx_t r = triangular_row(n);                     /* row number          */
    /* Compute the cumulative count of terms before row r.  */
    __int128 before = (__int128)(r - 1) * r / 2;       /* r(r‑1)/2            */
    idx_t k = n - (idx_t)before;                     /* column in the row   */

    /* Compute Fibonacci(k) * C(r, k).                     */
    seq_t f = fib(k);              /* Fibonacci number   */
    seq_t c = binomial(r, k);      /* Binomial coefficient*/

    return f * c;
}
