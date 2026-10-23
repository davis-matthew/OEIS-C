/*
 * A124852  Number of base 10 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A124852: Number of base‑10 circular n‑digit numbers whose
   adjacent digits differ by at most 2.
   Formula used: a(n) = trace(Aⁿ) where A is the 10×10 adjacency
   matrix A[i][j] = 1 if |i-j| ≤ 2, otherwise 0.
   (The graph has nodes 0…9 and edges between digits at distance ≤2.)
*/

/* No additional includes are required; the build supplies seq_t and idx_t. */

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 in the OEIS data */
    if (n == 0u) return (seq_t)1;

    /* cur holds A^(step-1); nxt will hold A^step. */
    seq_t cur[10][10];
    seq_t nxt[10][10];

    /* initialise cur = identity matrix (A⁰) */
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            cur[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* iterate step = 1 … n */
    for (idx_t step = 1; step <= n; ++step) {

        /* set nxt to zero */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                nxt[i][j] = (seq_t)0;

        /* cur × A → nxt  (A is fixed, adjacency 0/1) */
        for (int s = 0; s < 10; ++s) {
            for (int u = 0; u < 10; ++u) {
                seq_t val = cur[s][u];
                if (!val) continue;          /* skip zero entries */

                int start = u - 2;
                if (start < 0) start = 0;
                int end = u + 2;
                if (end > 9) end = 9;

                for (int v = start; v <= end; ++v) {
                    nxt[s][v] += val;
                }
            }
        }

        /* copy nxt back to cur for next iteration */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* trace of Aⁿ is sum of diagonal entries of cur */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < 10; ++i)
        sum += cur[i][i];

    return sum;
}
