/*
 * A049455  Triangle read by rows: T(n,k) = numerator of fraction in k-th term
 *          of n-th row of variant of Farey series.
 * https://oeis.org/A049455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, frac, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   stern_diagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) yields the numerators of the rows of the variant Farey series
 * (Stern–Brocot tree).  The sequence is obtained by concatenating the
 * rows of Stern's diatomic array.  If we denote the Stern sequence by
 * s(0)=0, s(1)=1, s(2k)=s(k), s(2k+1)=s(k)+s(k+1), then
 *
 *   a(n) = s(k)
 *
 * where k is the position of the n‑th numerator inside its row.
 * Rows have length 2^r + 1 for r = 0,1,2,...
 * The cumulative length up to and including row r is
 *
 *   C(r) = 2^(r+1) + r.
 *
 * For a given global index n (1‑based) we locate the smallest r
 * such that C(r) ≥ n, compute the offset
 *
 *   offset = n - C(r-1) - 1            (0‑based inside row r)
 *
 * and return s(offset).
 */

#include <stdint.h>
#include "../utils/stern_diagonal.h"

seq_t a(idx_t n)
{
    /* locate the row that contains the n-th term */
    idx_t cum = 0;       /* cumulative length of all previous rows */
    idx_t row = 0;
    while (1) {
        idx_t row_len = ((idx_t)1 << (unsigned)row) + 1;  /* 2^row + 1 */
        if (cum + row_len >= n)
            break;
        cum += row_len;
        ++row;
    }

    /* offset of the desired numerator inside its row (0‑based) */
    idx_t offset = n - cum - 1;
    return stern_diagonal(offset);
}
