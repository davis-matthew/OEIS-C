/*
 * A180860  Square array read by antidiagonals: T(m,n) is the Wiener index of
 *          the tadpole graph L(m,n) (m>=3, n>=1). L(m,n) is the graph
 *          obtained by joining with an edge a node in the cycle graph C_m to
 *          an end-node of the path graph P_n. The Wiener index of a connected
 *          graph is the sum of distances between a...
 * https://oeis.org/A180860
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   wiener_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/wiener_T.h"
/* a(n) = Wiener index of the tadpole graph L(m,n) listed by antidiagonals.
   For a given m,n (m>=3, n>=1)
       if m is even (m = 2h)
           T(m,n) = h^3 + (1/6)*n*(n*n-1) + h*n*(h + n + 1)
       if m is odd  (m = 2h+1)
           T(m,n) = (1/2)*h*(h+1)*(2h+1) + (1/6)*n*(n+1)*(n+2) + h*n*(h + n + 2)
   The index n is mapped to the pair (m,i) by reading the infinite square
   array T(m,i) by antidiagonals with offset 3 (so a(3) = T(3,1)). */

seq_t a(idx_t n)
{
    if (n < 3) {
        return 0;   /* indices below the sequence offset are undefined */
    }

    idx_t k = n - 3;   /* zero‑based position in the array */

    /* Find m such that m*(m+1)/2 > k, i.e. the diagonal index s = m+3 */
    idx_t m = 1;
    while (( (__int128)m * (m + 1) ) / 2 <= k) {
        ++m;
    }

    idx_t s = m + 3;

    /* Previous cumulative count T_prev = (m-1)*m/2 */
    idx_t prev = (m > 0 ? ( ( (__int128)(m - 1) * m ) / 2 ) : 0);

    idx_t pos = k - prev;        /* 0‑based offset inside the diagonal */

    idx_t i = (s - 1) - pos;     /* decreasing m ("row") */
    idx_t j = s - i;             /* corresponding n ("column") */

    return wiener_T(i, j);
}
