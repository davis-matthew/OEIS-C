/*
 * A153490  Sierpinski carpet, read by antidiagonals.
 * https://oeis.org/A153490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if for all k ≥ 0 either the k‑th base‑3 digit of i or
 *            of j is not 1, where (i,j) is the coordinates of the n‑th entry
 *            in the antidiagonal enumeration of the Sierpinski carpet matrix.
 *            a(n) = 0 otherwise. (Here the carpet matrix has a(1) = f(0,0) =
 *            1.) The computation is performed by first...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find diagonal s such that (s+1)(s+2)/2 ≥ n. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 < n)
        ++s;

    /* Number of entries before this diagonal. */
    idx_t before = s * (s + 1) / 2;

    /* Offset within the diagonal (0‑based). */
    idx_t offset = n - before - 1;

    idx_t i = offset;        /* row coordinate */
    idx_t j = s - offset;    /* column coordinate */

    /* Check for simultaneous 1 digits in any base‑3 position. */
    while (i || j) {
        if ((i % 3 == 1) && (j % 3 == 1))
            return (seq_t)0;
        i /= 3;
        j /= 3;
    }
    return (seq_t)1;
}
