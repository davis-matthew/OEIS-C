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
 * Helpers:   binom, eulerian
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"          /* binomial(n,k) returns seq_t */
#include "../utils/eulerian.h"

seq_t a(idx_t N)
{
    /* Find row r such that r*(r+1)/2 ≤ N < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= N) ++r;
    --r;                         /* now r is the row index */
    idx_t offset = (r * (r + 1)) / 2;
    idx_t col = N - offset;      /* column within the row */

    /* Binomial coefficient C(r, col). */
    seq_t bc = binom(r, col);

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
