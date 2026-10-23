/*
 * A181937  André numbers. Square array A(n,k), n>=2, k>=0, read by
 *          antidiagonals upwards, A(n,k) = n-alternating permutations of
 *          length k.
 * https://oeis.org/A181937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of n‑alternating permutations of length k, where
 *            (n,k) is obtained by reading the square array A(n,k) (n≥2,k≥0)
 *            by antidiagonals “upwards”. The sequence begins a(0)=1
 * Range:     u8 n <= 34, u16 n <= 76, u32 n <= 151, u64 n <= 349,
 *            u128 n <= 818, big any n
 * Helpers:   count_andre
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/count_andre.h"

idx_t _floor_log2_uint64(idx_t v); /* forward declaration if needed */

/* Main sequence function */
seq_t a(idx_t N)
{
    /* Map index N to (row=n, col=k) in the antidiagonal reading. */
    unsigned long long idx = N;
    long double val = sqrtl(1.0L + 8.0L * (long double)idx);
    unsigned long long d = (unsigned long long)((1.0L + val) / 2.0L) + 1ULL; /* diagonal number */

    unsigned long long prev = (d - 1ULL) * (d - 2ULL) / 2ULL;   /* cumulative count up to previous diagonal */
    unsigned long long pos  = idx - prev;                        /* offset within diagonal, 0‑based */

    unsigned long long n_row = d - pos;    /* n = row index */
    unsigned long long k_len = pos;        /* k = column index */

    return count_andre((int)n_row, (int)k_len);
}
