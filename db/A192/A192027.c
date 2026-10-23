/*
 * A192027  Square array read by antidiagonals: W(n,m) (n >= 1, m >= 1) is the
 *          Wiener index of the graph G(n,m) obtained from the n-circuit graph
 *          by adjoining m pendant edges at each node of the circuit.
 * https://oeis.org/A192027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A192027:  a(n) = W(n_row, n_col)
   W(n,m) is the Wiener index of graph G(n,m).

   The sequence is the list of all W(n,m) read by antidiagonals of the
   infinite square array W(n,m) (n>=1, m>=1).  For each diagonal with
   constant sum s=n+m the terms are taken in the order of decreasing
   m (equivalently increasing n), i.e. (s-1,1), (s-2,2), … , (1,s-1).
   The i-th term in the sequence is obtained by mapping i to a pair
   (n,m) as follows:

     let s be the smallest integer with (s-1)*s/2 >= i;
     let prev = (s-2)*(s-1)/2        // number of terms before diagonal s
     let offset = i - prev - 1      // 0‑based index inside diagonal s
     let m = offset + 1
     let n = s - offset - 1

   Then a(i) = W(n,m).  The Wiener index is given by
     * W(1,m) = m^2
     * for even n: W = n*(n^2/4 + 2*m^2*n + m^2*n^2/4 + 2*m*n
                     + m*n^2/2 - 2*m)/2
     * for odd  n: W = n*(n^2-1 + m^2*n^2 + 8*m^2*n - m^2
                     + 2*m*n^2 + 8*m*n - 10*m)/8
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t idx) {
    if (idx == 0) return 0;  // not used, but safe

    /* Find diagonal s such that (s-1)*s/2 >= idx.   */
    idx_t s = 2;
    while (true) {
        unsigned __int128 tot = (unsigned __int128)(s - 1) * s / 2;
        if (tot >= idx) break;
        ++s;
    }

    /* Number of terms before diagonal s */
    unsigned __int128 prev128 = (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t prev = (idx_t)prev128;

    /* offset inside diagonal (0‑based) */
    idx_t offset = idx - prev - 1;

    /* Coordinates (n,m) */
    idx_t m = offset + 1;
    idx_t n = s - offset - 1;

    /* Compute Wiener index W(n,m) */
    seq_t N = (seq_t)n;
    seq_t M = (seq_t)m;

    /* Special case n = 1 */
    if (N == 1) return M * M;

    if ((N & 1) == 0) {           /* even n */
        seq_t n2 = N * N;
        seq_t termA = n2 / 4;
        seq_t termB = 2 * M * M * N;
        seq_t termC = M * M * n2 / 4;
        seq_t termD = 2 * M * N;
        seq_t termE = M * n2 / 2;
        seq_t termF = 2 * M;
        seq_t inside = termA + termB + termC + termD + termE - termF;
        return N * inside / 2;
    } else {                      /* odd n */
        seq_t n2 = N * N;
        seq_t termA = n2 - 1;
        seq_t termB = M * M * n2;
        seq_t termC = 8 * M * M * N;
        seq_t termD = M * M;
        seq_t termE = 2 * M * n2;
        seq_t termF = 8 * M * N;
        seq_t termG = 10 * M;
        seq_t inside = termA + termB + termC - termD + termE + termF - termG;
        return N * inside / 8;
    }
}
