/*
 * A201509  Irregular triangle read by rows: T(n,k) = 2*T(n-1,k) + T(n-2,k-1)
 *          with T(0,0) = 0, T(n,0) = T(1,1) = 1 and T(n,k) = 0 if k < 0 or if
 *          n < k.
 * https://oeis.org/A201509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the sequence of non‑zero entries of the irregular
 *            triangle T(n,k) read by rows. Triangle definition (OEIS
 *            A201509): T(0,0) = 0, T(n,0) = 1 for n ≥ 1, T(1,1) = 1, T(n,k) =
 *            2*T(n-1,k) + T(n-2,k-1) for k > 0, T(n,k) = 0 if k < 0 or n < k.
 *            a(n) lists all T(n,k) > 0 in the order T(1,0), T(1,1)...
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_ROW_SIZE 1000  /* wide enough for the required number of rows for 10 000 terms */

seq_t a(idx_t n)
{
    /* Row buffers.  rows[0] = previous‑previous row (n-2),
     * rows[1] = previous row (n-1),
     * rows[2] = current row (n).
     * lens[i] holds the number of valid entries in rows[i].
     */
    static seq_t rows[3][MAX_ROW_SIZE];
    static size_t lens[3];

    /* Initialize row 0: n = 0 */
    lens[0] = 1;
    rows[0][0] = 0;

    /* Row 1: n = 1  (two entries) */
    lens[1] = 2;
    rows[1][0] = 1;
    rows[1][1] = 1;

    /* Count of terms produced so far (0‑based index into a). */
    size_t term_index = 0;

    /* Process row 1 */
    for (idx_t k = 0; k < (idx_t)lens[1]; ++k) {
        if (rows[1][k] > 0) {
            if (term_index == (size_t)n) return rows[1][k];
            ++term_index;
        }
    }

    /* Generate subsequent rows until we reach the asked index. */
    idx_t cur_n = 2;               /* next row number to compute */
    while (1) {
        /* Prepare the new row.  It has indices 0 .. cur_n-1. */
        lens[2] = cur_n;

        for (idx_t k = 0; k < lens[2]; ++k) {
            seq_t val = 0;

            /* 2 * T(n-1, k)  – use prev row where defined */
            if (k < (idx_t)lens[1]) {
                val = rows[1][k] + rows[1][k];
            }

            /* + T(n-2, k-1)  – use prev‑prev row where defined */
            if (cur_n >= 2 && k >= 1 && (k - 1) < (idx_t)lens[0]) {
                val += rows[0][k - 1];
            }

            rows[2][k] = val;
        }

        /* Output the non‑zero elements of the current row. */
        for (idx_t k = 0; k < lens[2]; ++k) {
            if (rows[2][k] > 0) {
                if (term_index == (size_t)n) return rows[2][k];
                ++term_index;
            }
        }

        /* Rotate the rows: prevprev <- prev, prev <- current. */
        lens[0] = lens[1];
        for (idx_t i = 0; i < lens[0]; ++i) {
            rows[0][i] = rows[1][i];
        }

        lens[1] = lens[2];
        for (idx_t i = 0; i < lens[1]; ++i) {
            rows[1][i] = rows[2][i];
        }

        ++cur_n;
    }
}
