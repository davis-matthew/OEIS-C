/*
 * A058127  Triangle read by rows: T(j,k) is the number of acyclic functions
 *          from {1,...,j} to {1,...,k}. For n >= 1, a(n) = (k-j)*k^(j-1),
 *          where k is such that C(k,2) < n <= C(k+1,2) and j = (n-1) mod
 *          C(k,2). Alternatively, table T(k,j) read by antidiagonals with k
 *          >= 1, 0 <= j <= k: T(k,j) = number of acyc...
 * https://oeis.org/A058127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, tabl, easy
 * Formula:   a(n) = 1 for n = (k-1)·k/2 + 1 (i.e. when (n-1) mod (k·(k-1)/2)
 *            = 0); otherwise a(n) = (k - j)·k^(j-1) where k =
 *            ceil((sqrt(8·n+1) - 1)/2) and j = (n-1) mod (k·(k-1)/2)
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 76, u64 n <= 170,
 *            u128 n <= 434, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Special case for the very first term */
    if (n == 1)
        return 1;

    /* Find the smallest k such that k*(k+1)/2 >= n. */
    idx_t k = 1;
    while ((k * (k + 1) / 2) < n)
        ++k;

    /* m = C(k,2) = k*(k-1)/2  (must be > 0 for n > 1) */
    idx_t m = k * (k - 1) / 2;

    /* j = (n-1) mod m  (m > 0 here) */
    idx_t j = (n - 1) % m;

    /* For j == 0 the value is 1 (T(k,0)) */
    if (j == 0)
        return 1;

    /* Compute k^(j-1) and multiply by (k-j) */
    seq_t powk = pow_int((seq_t)k, j - 1);
    return ((seq_t)(k - j)) * powk;
}
