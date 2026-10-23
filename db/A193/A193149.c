/*
 * A193149  Numbers of spanning trees of the odd graphs.
 * https://oeis.org/A193149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = [ ∏_{j=1}^{n-1} (n - (-1)^j·C(n-j,
 *            n-1-j))^{C(2n-1,j)-C(2n-1,j-1)} ] / C(2n-1,n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, binom, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n <= 1) return seq_one();       /* a(1) = 1, lower offset irrelevant */

    idx_t k = n;                         /* use k for the graph order */

    /* number of vertices of the odd graph O_k */
    seq_t v = binom(2*k - 1, k - 1);

    seq_t prod = seq_one();

    for (idx_t j = 1; j < k; ++j) {
        /* magnitude of the j‑th eigenvalue of the adjacency matrix */
        seq_t c = binom(k - j, (k - 1) - j);   /* C(k-j, k-1-j) */

        seq_t l_j;
        if ((j & 1) == 0) {          /* even j: λ_j = +c, so l_j = k - c */
            l_j = (seq_t)k - c;
        } else {                     /* odd j: λ_j = -c, so l_j = k + c */
            l_j = (seq_t)k + c;
        }

        /* multiplicity of this eigenvalue in the Laplacian spectrum */
        seq_t m_j = binom(2*k - 1, j) - binom(2*k - 1, j - 1);

        /* multiply by l_j ^ m_j */
        prod = prod * big_pow(l_j, m_j);
    }

    /* divide by the number of vertices to obtain the number of spanning trees */
    return prod / v;
}
