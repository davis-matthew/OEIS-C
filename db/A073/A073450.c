/*
 * A073450  Triangle T(j,k) = remainder of Fibonacci(j) divided by
 *          Fibonacci(k), for 3 < j and 2 < k < j.
 * https://oeis.org/A073450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = Fib(j) mod Fib(k) where j = m + 3, k = 3 + (n -
 *            (m-1)*m/2) and m is the smallest integer satisfying m*(m+1)/2 >
 *            n (thus enumerating the triangle T(j,k) row-wise)
 * Range:     u8 n <= 55, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Find the minimal m with m*(m+1)/2 > n.  */
    idx_t low = 1, high = 1;
    while ((__int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;              /* m >= 1 */

    /* Compute j and k for the requested n. */
    idx_t j = m + 3;
    idx_t prev = ( (m - 1) * m ) / 2;        /* start index of this row */
    idx_t k = 3 + (n - prev);

    /* Compute the Fibonacci values and return the remainder. */
    seq_t Fj = fib(j);
    seq_t Fk = fib(k);
    return Fj % Fk;
}
