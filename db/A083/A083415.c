/*
 * A083415  Triangle read by rows: T(n,k) is defined as follows. Write the
 *          numbers from 1 to n^2 consecutively in n rows of length n; T(n,k)
 *          = number of primes in k-th row.
 * https://oeis.org/A083415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of primes in the k‑th row of the triangle * where
 *            n is the index into the flattened list of rows. The * triangle
 *            is defined by writing the integers 1..m² in m rows of * length
 *            m; T(m,k) = number of primes in row k. The sequence *
 *            a(1),a(2),… is the triangle read row by row. This * i...
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrime.h"

/* Find minimal r such that r*(r+1)/2 >= n */
static idx_t triangular_row(idx_t n)
{
    /* Binary search on row number */
    idx_t low = 1, high = 2;
    while ( ((__int128)high * (high + 1)) / 2 < n ) high <<= 1;
    while ( low < high ) {
        idx_t mid = low + (high - low) / 2;
        if ( ((__int128)mid * (mid + 1)) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find row (here called uint) and its position within the row */
    idx_t row = triangular_row(n);
    idx_t prev_tri = (row > 1) ? ((row - 1) * row) / 2 : 0;
    idx_t k = n - prev_tri;          /* 1‑based column index within the row */

    /* Sub‑interval of numbers in this row */
    idx_t L = (k - 1) * row + 1;      /* start value of the row */
    idx_t R = k * row;               /* end   value of the row */

    seq_t count = 0;
    for (idx_t i = L; i <= R; ++i) {
        if (isPrime(i))
            ++count;
    }
    return count;
}
