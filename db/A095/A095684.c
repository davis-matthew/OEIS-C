/*
 * A095684  Triangle read by rows. There are 2^(m-1) rows of length m, for m =
 *          1, 2, 3, ... The rows are in lexicographic order. The rows have
 *          the property that the first entry is 1, the second distinct entry
 *          (reading from left to right) is 2, the third distinct entry is 3,
 *          etc.
 * https://oeis.org/A095684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the flattened sequence of the restricted‑growth strings
 *            * (RGS) of length m, enumerated lexicographically. Each RGS
 *            consists of * m entries; the first is 1, subsequent entries are
 *            either equal to the * previous value or one larger. * For a
 *            given n, the algorithm: * 1. Find the length m su...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the n-th element of the triangular array A095684,
 * flattened row‑by‑row (each element is a single integer). */
seq_t a(idx_t n)
{
    /* 1. Find row length m and cumulative terms up to previous length. */
    idx_t m = 1;                          /* current row length   */
    idx_t row_count = 1;                  /* number of rows of
                                             length m ( = 2^(m-1) )  */
    idx_t terms_total = 0;                /* terms before current m */

    while (1) {
        /* terms contributed by all rows of current length m */
        idx_t inc = m * row_count;
        if (terms_total + inc >= n) {
            break;                       /* m is the desired length */
        }
        terms_total += inc;
        row_count <<= 1;                  /* next row count = 2^(m)  */
        m++;
    }

    /* 2. Locate offset within the rows of length m */
    idx_t offset = n - terms_total - 1;   /* zero‑based index in [0, m*row_count-1] */
    idx_t row_index = offset / m;         /* which row (0‑based)   */
    idx_t pos_in_row = offset % m;        /* position in that row (0‑based) */

    /* 3. Compute value at the requested position */
    if (pos_in_row == 0) {
        return (seq_t)1;                  /* first entry is always 1 */
    }

    seq_t cur = 1;                        /* current value */
    /* positions are 1‑based internally; we need to process positions
     * 2 .. pos_in_row+1 to get to the required element. */
    for (idx_t pos = 2; pos <= pos_in_row + 1; pos++) {
        /* bit corresponding to this position in row_index */
        idx_t bit = m - pos;              /* bit 0 is the least
                                               significant (position m)   */
        if (row_index & ((idx_t)1 << bit)) {
            cur++;                        /* introduce a new value */
        }
    }
    return cur;
}
