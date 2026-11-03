/*
 * A174914  A symmetrical triangle sequence T(n, m) = 2 + binomial(n, m)^3 -
 *          3*binomial(n, m)*Eulerian(n+1, m) + Eulerian(n+1, m)^3.
 * https://oeis.org/A174914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = 2 + C(n,m)^3 - 3*C(n,m)*E(n+1,m) + E(n+1,m)^3 a(n) is
 *            the linear (row‑major) indexing of the triangle T
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 58, u128 n <= 141,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 5141 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static seq_t eulerian(idx_t n, idx_t k)
{
    /* Eulerian(n,k) = A008292(n,k),  n≥1, 0≤k<n.  Use the triangular
       recurrence:  E(n,k) = (k+1)E(n-1,k) + (n-k)E(n-1,k-1),
       with E(1,0)=1.  Row lengths are very small (≤ 200). */
    if (k >= n) return 0;
    if (n == 1) return 1;            /* only element 0 exists */

    /* Fixed‑size arrays; 200 > max n needed for n up to 10 000. */
    static seq_t prev[200];
    static seq_t curr[200];

    /* Base row 1: E(1,0)=1.  All other entries stay zero. */
    for (idx_t i = 0; i < 200; ++i) prev[i] = 0;
    prev[0] = 1;

    for (idx_t row = 2; row <= n; ++row)
    {
        for (idx_t j = 0; j < row; ++j)
        {
            seq_t term1 = 0;
            if (j <= row - 2) term1 = prev[j];
            term1 = (seq_t)(j + 1) * term1;

            seq_t term2 = 0;
            if (j > 0) term2 = prev[j - 1];
            term2 = (seq_t)(row - j) * term2;

            curr[j] = term1 + term2;
        }
        /* Copy current row to prev for next iteration. */
        for (idx_t j = 0; j < row; ++j) prev[j] = curr[j];
    }
    return prev[k];
}

seq_t a(idx_t N)
{
    /* Find row r such that r*(r+1)/2 ≤ N < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= N) ++r;
    --r;                         /* now r is the row index */
    idx_t offset = (r * (r + 1)) / 2;
    idx_t col = N - offset;      /* column within the row */

    /* Binomial coefficient C(r, col). */
    seq_t bc = binomial(r, col);

    /* Eulerian number E(r+1, col). */
    seq_t ea = eulerian(r + 1, col);

    /* Compute T(r, col) = 2 + bc^3 - 3*bc*ea + ea^3. */
    seq_t bc2 = bc * bc;
    bc2 *= bc;          /* bc^3 */

    seq_t ea2 = ea * ea;
    ea2 *= ea;          /* ea^3 */

    seq_t prod = 3 * bc;   /* 3*bc */
    prod *= ea;           /* 3*bc*ea */

    seq_t val = 2 + bc2 - prod + ea2;
    return val;
}
