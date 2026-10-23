/*
 * A182830  Array of the numbers (3*i+2)*3^j, i>=0, j>=0, read by
 *          antidiagonals.
 * https://oeis.org/A182830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (3*i + 2) * 3^j, where (i, j) are the coordinates of n
 *            read diagonally in the array T(i, j) = (3*i + 2) * 3^j, with i
 *            >= 0, j >= 0
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the nth term of OEIS sequence A182830 (1‑based index). */
seq_t a(idx_t n)
{
    /* Handle the unlikely case of n == 0 (offset is 1). */
    if (n == 0) {
        return 0;
    }

    /* Locate the diagonal d (where i + j = d) that contains n. */
    idx_t rem = n;      /* remaining index inside current cumulative count   */
    idx_t d = 0;        /* current diagonal number (starts at 0)             */
    idx_t count = 1;    /* number of elements in current diagonal (d+1)     */

    while (rem > count) {
        rem -= count;   /* skip entire diagonal                       */
        d++;            /* move to next diagonal                        */
        count = d + 1;  /* update element count for new diagonal       */
    }

    /* Position within this diagonal: offset from 0 to d. */
    idx_t offset = rem - 1;          /* 0‑based offset in the diagonal    */
    idx_t i = offset;                /* row index                          */
    idx_t j = d - offset;            /* column index                       */

    /* Compute (3*i + 2) * 3^j.  Use loops to avoid overflow where possible. */
    seq_t coeff = (seq_t)(3 * i + 2);
    seq_t pow3 = 1;
    for (idx_t k = 0; k < j; ++k) {
        pow3 *= 3;
    }

    return coeff * pow3;
}
