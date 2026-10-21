/*
 * A211701  Rectangular array by antidiagonals, n >= 1, k >= 1: R(n,k) = n +
 *          [n/2] + ... + [n/k], where [ ]=floor.
 * https://oeis.org/A211701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = R(m,k) where (m,k) are the coordinates obtained by
 *            reading the 2‑D array of R(m,k) = sum_{i=1}^k floor(m / i) along
 *            antidiagonals. In other words, the sequence enumerates the
 *            values of R(m,k) in the order (1,1), (2,1), (1,2), (3,1), (2,2),
 *            (1,3), ... with m = (d-1)-offset, k = 1+offset for...
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* not used but safe */

    /* Find the diagonal 'd' such that (d-1)*d/2 >= n;
       minimal such d. */
    idx_t d = 2;
    while ( ((unsigned __int128)(d - 1) * d) / 2 < (unsigned __int128)n )
        d++;

    /* Count of elements before this diagonal */
    unsigned __int128 prev_cnt = ((unsigned __int128)(d - 2) * (d - 1)) / 2;
    idx_t offset = n - (idx_t)prev_cnt - 1;   /* 0‑based within the diagonal */

    /* Coordinates (m,k) for this element */
    idx_t m = (d - 1) - offset;             /* m >= 1 */
    idx_t k = 1 + offset;                   /* k >= 1 */

    /* Compute R(m,k) = sum_{i=1}^k floor(m / i) */
    seq_t result = (seq_t)0;
    seq_t m_seq = (seq_t)m;

    for (idx_t i = 1; i <= k; ++i)
        result += m_seq / (seq_t)i;

    return result;
}
