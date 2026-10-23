/*
 * A192028  Square array read by antidiagonals: W(n,m) (n >= 1, m >= 1) is the
 *          Wiener index of the graph G(n,m) obtained from the n-circuit graph
 *          by joining at each of its nodes a path with m nodes (n >= 1, m >=
 *          1; if m=1, then the n-circuit is not modified).
 * https://oeis.org/A192028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = W(row,col) where row,col are taken from the square array
 *            * W(n,m) read by reversed antidiagonals. For W(n,m) use: * if n
 *            even: W = (n*m*(3*m*n^2 + 12*n*m^2 - 8*m^2 - 12*n*m + 12*m -
 *            4))/24 * if n odd : W = (n*m*(3*m*n^2 + 12*n*m^2 - 8*m^2 -
 *            12*n*m + 9*m - 4))/24
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find the diagonal s such that s(s-1)/2 >= n */
    idx_t s = 1;
    while ((idx_t)(((unsigned __int128)(s - 1) * s) / 2) < n)
        s++;

    /* number of terms before this diagonal */
    idx_t prev = (idx_t)(((unsigned __int128)(s - 2) * (s - 1)) / 2);

    /* position inside diagonal (1-based) */
    idx_t pos = n - prev;          /* 1 <= pos <= s-1 */
    idx_t row = s - pos;           /* n in W(n,m) */
    idx_t col = pos;               /* m in W(n,m) */

    seq_t n_seq = (seq_t)row;
    seq_t m_seq = (seq_t)col;

    seq_t inside;
    if ((row & 1) == 0) {  /* n even */
        inside =
            (seq_t)3 * m_seq * n_seq * n_seq
          + (seq_t)12 * n_seq * m_seq * m_seq
          - (seq_t)8 * m_seq * m_seq
          - (seq_t)12 * n_seq * m_seq
          + (seq_t)12 * m_seq
          - (seq_t)4;
    } else {               /* n odd */
        inside =
            (seq_t)3 * m_seq * n_seq * n_seq
          + (seq_t)12 * n_seq * m_seq * m_seq
          - (seq_t)8 * m_seq * m_seq
          - (seq_t)12 * n_seq * m_seq
          + (seq_t)9 * m_seq
          - (seq_t)4;
    }

    seq_t numerator = n_seq * m_seq * inside;
    seq_t result = numerator / (seq_t)24;
    return result;
}
