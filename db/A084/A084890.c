/*
 * A084890  Triangular array, read by rows: T(n,k) = arithmetic derivative of
 *          n*k, 1<=k<=n.
 * https://oeis.org/A084890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = arithmetic derivative of ( row_of_index(n) *
 *            column_of_index(n) ), where the triangular array T(n,k) =
 *            A003415(n*k) is read by rows: index(1) = T(1,1), index(2) =
 *            T(2,1), index(3) = T(2,2), index(4) = T(3,1),
 * Range:     u8 n <= 73, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   arithmetic_derivative
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/arithmetic_derivative.h"

/* Return the term a(n) of the sequence A084890. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n.
       r is the smallest integer with that property. */
    idx_t r = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);
    if ((unsigned __int128)r * (r + 1) / 2 < n) r++;

    /* Compute column k:   k = n - (r-1)*r/2   with 1 <= k <= r */
    idx_t prev = (idx_t)((unsigned __int128)(r - 1) * r / 2);
    idx_t k = n - prev;

    /* Compute the arithmetic derivative of r*k. */
    unsigned __int128 prod128 = (unsigned __int128)r * k;
    /* For the limits of the test suite prod128 fits into 64 bits. */
    uint64_t prod = (uint64_t)prod128;

    return arithmetic_derivative(prod);
}
