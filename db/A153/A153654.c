/*
 * A153654  Triangle T(n, k, j) = T(n-1, k, j) + T(n-1, k-1, j) + (2*j +
 *          1)*prime(j)*T(n-2, k-1, j) with T(2, k, j) = prime(j) and j = 9,
 *          read by rows.
 * https://oeis.org/A153654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) reads the rows of the triangle T(n,k,9) defined by *
 *            T(1,1,9)=2, T(2,1,9)=T(2,2,9)=23, * T(3,2,9)=2*23^2-4, *
 *            T(4,2,9)=T(4,3,9)=23^3-2, * for n≥5:
 *            T(n,k,9)=T(n‑1,k,9)+T(n‑1,k‑1,9)+437*T(n‑2,k‑1,9), * with
 *            borders T(n,1,9)=T(n,n+1,9)=2. * a(n) is the triangle read by
 *            rows (offset 1)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 39, u64 n <= 127, u128 n <= 449,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for j = 9: prime(9) = 23, (2*j+1)*prime(j) = 19*23 = 437 */
    const seq_t prime = 23;
    const seq_t coeff = 437;

    /* maximum number of rows needed for n up to 10000 ( ≈141 rows ) */
    const idx_t MAX_ROW = 200;

    /* three buffers used for rotating rows */
    seq_t buf0[MAX_ROW];
    seq_t buf1[MAX_ROW];
    seq_t buf2[MAX_ROW];

    /* indices of buffers: prev2, prev1, cur */
    idx_t idx_prev2 = 0;
    idx_t idx_prev1 = 1;
    idx_t idx_cur   = 2;

    seq_t *prev2 = &buf0[0];
    seq_t *prev1 = &buf1[0];
    seq_t *cur   = &buf2[0];

    idx_t cumulative = 0;   /* number of elements processed so far */

    /* generate rows starting from row = 1 */
    for (idx_t row = 1; ; ++row)
    {
        idx_t size;                     /* length of current row (row number) */
        size = row;

        /* fill the current row according to the definition */
        if (row == 1)
        {
            cur[0] = prime * 0 + 2;     /* 2 */
        }
        else if (row == 2)
        {
            cur[0] = prime;
            cur[1] = prime;
        }
        else if (row == 3)
        {
            cur[0] = 2;
            cur[1] = 2 * prime * prime - 4;
            cur[2] = 2;
        }
        else if (row == 4)
        {
            cur[0] = 2;
            cur[1] = prime * prime * prime - 2;
            cur[2] = cur[1];
            cur[3] = 2;
        }
        else /* row >=5: use recurrence */
        {
            cur[0] = 2;
            cur[size - 1] = 2;
            for (idx_t k = 1; k < size - 1; ++k)
            {
                cur[k] = prev1[k] + prev1[k - 1] + coeff * prev2[k - 1];
            }
        }

        /* check if the target index n lies in this row */
        idx_t start_index = cumulative + 1;
        if (n >= start_index && n < start_index + size)
        {
            idx_t pos = n - start_index;      /* 0‑based column in this row */
            return cur[pos];
        }

        /* update cumulative count */
        cumulative += size;

        /* rotate buffers: prev2 <- prev1, prev1 <- cur, cur <- prev2 */
        seq_t *tmp = prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = tmp;
    }
}
