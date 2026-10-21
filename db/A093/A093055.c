/*
 * A093055  Triangle T(j,k) read by rows, where T(j,k) = number of
 *          non-singleton cycles in the in-situ transposition of a rectangular
 *          j X k matrix.
 * https://oeis.org/A093055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of non‑singleton cycles in the in‑situ *
 *            transposition of a j×k matrix read by rows (j≥k>1). The * n‑th
 *            term is obtained by mapping the index n to the pair (j,k) * as
 *            described in the OEIS entry. This implementation finds j and *
 *            k, builds the permutation induced by the transpose, a...
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find j such that S(j-1) < n ≤ S(j), where S(j) = (j-1)·j/2. */
    idx_t low = 2, high = 2;
    while ((unsigned __int128)(high - 1) * high / 2 < n) {
        high <<= 1;
        if (high == 0) {              /* avoid overflow of 64‑bit shift */
            high = ~((idx_t)0);
            break;
        }
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 smid = (unsigned __int128)(mid - 1) * mid / 2;
        if (smid >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t j = low;

    /* Compute k from n.  Let P(j) = (j-2)(j-1)/2 be the number of
     * terms before the j‑th row. */
    unsigned __int128 prev = (unsigned __int128)(j - 2) * (j - 1) / 2;
    idx_t offset = (idx_t)(n - (idx_t)prev - 1);   /* 0‑based offset in row */
    idx_t k = offset + 2;                         /* 2‑based column index */

    /* Size of the matrix and the permutation. */
    size_t J = (size_t)j;
    size_t K = (size_t)k;
    size_t N = J * K;          /* number of elements in the matrix */

    /* Visited flag array.  Allocate with calloc to initialise to zero. */
    unsigned char *visited = (unsigned char *)calloc(N, 1);
    if (!visited)
        return (seq_t)0;       /* allocation failure – should never occur */

    seq_t cycles = 0;

    for (size_t p = 0; p < N; ++p) {
        if (!visited[p]) {
            size_t curr = p;
            visited[curr] = 1;
            size_t len = 1;
            while (1) {
                size_t i = curr / K;      /* row index (0‑based)   */
                size_t r = curr % K;      /* column index (0‑based)*/
                size_t next = r * J + i;  /* transposed position   */
                if (next == p)
                    break;
                visited[next] = 1;
                curr = next;
                ++len;
            }
            if (len > 1)
                ++cycles;
        }
    }

    free(visited);
    return cycles;
}
