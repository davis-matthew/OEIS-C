/*
 * A191449  Dispersion of (3,6,9,12,15,...), by antidiagonals.
 * https://oeis.org/A191449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ⌊(3*i-1)/2⌋ · 3^(j-1) where (i,j) are the row and column
 *            of the n‑th element in the array T(i,j)=⌊(3*i-1)/2⌋·3^(j-1) read
 *            anti‑diagonally starting at T(1,1)
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow3.h"

/* fast exponentiation for big integer base 3 */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but return 0 for safety */
    /* Find the diagonal d (i+j) that contains the n‑th element.
       The k-th diagonal has k-1 elements, and the cumulative count up
       to diagonal d is (d-1)*d/2. */
    idx_t d = 2;
    while ((d - 1) * d / 2 < n) ++d;

    /* Number of elements before diagonal d */
    idx_t before = (d - 2) * (d - 1) / 2;
    /* Offset within the diagonal (0‑based) */
    idx_t offset = n - before - 1;
    idx_t i = offset + 1;
    idx_t j = d - i;

    seq_t first = ((seq_t)(3 * i - 1)) / 2;   /* ⌊(3i-1)/2⌋ */
    seq_t power = pow3(j - 1);                /* 3^(j-1) */
    return first * power;
}
