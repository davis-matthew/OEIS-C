/*
 * A109692  Triangle of coefficients in expansion of
 *          (1+x)*(1+3x)*(1+5x)*(1+7x)*...*(1+(2n-1)x).
 * https://oeis.org/A109692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient T(r,c) in the triangle of coefficients
 *            of * (1+x)(1+3x)(1+5x)...(1+(2r-1)x). r and c are obtained from
 *            n by * r = floor((sqrt(8*n+1)-1)/2), c = n - r*(r+1)/2. *
 *            Recurrence for the triangle: T(0,0)=1; T(r,0)=1; T(r,r)=(2r-1)!!
 *            ; * T(r,c)=T(r-1,c)+(2r-1)*T(r-1,c-1). This fun...
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   int128_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/int128_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Determine row r and column c of the flattened triangle. */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;
    idx_t fsqrt = int128_sqrt(t);           /* floor(sqrt(8*n+1)) */
    idx_t r = (fsqrt - 1) / 2;              /* row index */
    idx_t prev = r * (r + 1) / 2;           /* number of elements before this row */
    idx_t c = n - prev;                     /* column within the row */

    /* Allocate column array for the current row, zero‑initialised. */
    seq_t *row = (seq_t*)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!row)
        return (seq_t)0;                   /* out of memory */

    row[0] = (seq_t)1;                     /* base case */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t factor = (seq_t)(2 * i - 1);
        for (idx_t j = i; j > 0; --j) {
            row[j] = row[j] + factor * row[j - 1];
        }
    }

    seq_t result = row[c];
    free(row);
    return result;
}
