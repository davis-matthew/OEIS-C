/*
 * A203990  Symmetric matrix based on f(i,j) = (i+j)*min(i,j), by
 *          antidiagonals.
 * https://oeis.org/A203990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 263, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   diagonal_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203990: Enumerate the symmetric matrix
   with entries f(i,j) = (i+j)*min(i,j) along anti‑diagonals.
   a(1)=2, a(2)=3, a(3)=3, a(4)=4, …   (offset 1) */

/* Size of indices and values are determined by the build system.
   No uses of <math.h> or floating point are required. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/diagonal_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, return 0 */

    /* Determine which anti‑diagonal contains the n‑th element. */
    idx_t s = diagonal_sum(n);        /* s = i + j of the diagonal */

    /* Number of elements before this diagonal: (s-1)*(s-2)/2. */
    idx_t prev = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t offset = n - prev - 1;     /* position on the diagonal (0‑based) */

    /* Compute the row and column indices. */
    idx_t i = offset + 1;            /* rows start at 1 */
    idx_t j = s - i;                 /* columns start at 1 */

    /* Compute f(i,j) = (i+j)*min(i,j). */
    seq_t iv = (seq_t)i;
    seq_t jv = (seq_t)j;
    seq_t sum = iv + jv;             /* equals (seq_t)s */
    seq_t min_ij = iv < jv ? iv : jv;

    return sum * min_ij;
}
