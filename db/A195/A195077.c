/*
 * A195077  Interspersion fractally induced by A009620, a rectangular array,
 *          by antidiagonals.
 * https://oeis.org/A195077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A195077.
   The sequence is read by antidiagonals from the array
   A009620.  For row i (1‑based) and column j (1‑based) the entry is

       a(i,j) = Tj + (i‑1)*i/2 + ⌊i/3⌋ + (j‑1)*(i‑1 – ((i % 3) == 2)),

   where Tj = j*(j+1)/2 is the j‑th triangular number.
   The function below converts a 1‑based index n into the appropriate
   (i,j) pair and evaluates the above formula. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* determine the anti‑diagonal (sum of indices) that contains n */
    idx_t s = 2;                     /* s = i + j */
    while (s * (s - 1) / 2 < n) {
        ++s;
    }

    /* number of elements before this diagonal */
    idx_t prev = (s - 1) * (s - 2) / 2;
    idx_t i = n - prev;              /* row index, 1‑based */
    idx_t j = s - i;                 /* column index, 1‑based */

    /* use seq_t for the arithmetic to avoid truncation with large types */
    seq_t i_seq = (seq_t)i;
    seq_t j_seq = (seq_t)j;

    /* triangular number T_j = j*(j+1)/2 */
    seq_t Tj = j_seq * (j_seq + 1) / 2;

    /* offset for column 1 of row i */
    seq_t offset_i1 = ((i_seq - 1) * i_seq) / 2 + (seq_t)(i / 3);

    /* coefficient of (j‑1) depends on i mod 3 */
    seq_t c = i_seq - 1;
    if (i % 3 == 2) {
        c -= 1;
    }

    /* total offset */
    seq_t offset = offset_i1 + (j_seq - 1) * c;

    return Tj + offset;
}
