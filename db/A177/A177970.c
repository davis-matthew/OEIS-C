/*
 * A177970  Array T(n,m) = A177944(2*n,2*m) read by antidiagonals.
 * https://oeis.org/A177970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, look
 * Formula:   a(n) = T(i,j) where (i,j) is the n‑th element read by
 *            antidiagonal from the table T(i,j) = binom(2*i+2*j+1,
 *            2*i)*(2*j+1) - 2*i - 2*j
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 126, u64 n <= 509,
 *            u128 n <= 2043, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the supplied binom(a,b) routine from "../utils/binom.h". */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the diagonal s such that the n-th element lies in that diagonal.
       For diagonal s, there are (s+1) elements, and the total number of
       elements seen up to, but not including, diagonal s is s*(s+1)/2.
       We keep incrementing s until the cumulative count exceeds n. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    idx_t t_prev = s * (s + 1) / 2;          /* elements before diagonal s   */
    idx_t k      = n - t_prev;               /* offset within the diagonal   */
    idx_t i      = k;                        /* row index                    */
    idx_t j      = s - k;                    /* column index                 */

    /* Compute T(i,j) = binom(2*i + 2*j + 1, 2*i) * (2*j + 1) - (2*i + 2*j) */
    seq_t val = binom(2 * i + 2 * j + 1, 2 * i);
    val = val * (seq_t)(2 * j + 1);
    val = val - (seq_t)(2 * i + 2 * j);

    return val;
}
