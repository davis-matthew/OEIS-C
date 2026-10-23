/*
 * A174693  Triangle read by rows: T(n,k) = n! * binomial(n, k) * Eulerian(n +
 *          1, k) - n! + 1.
 * https://oeis.org/A174693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   T(n,k) = n! * binomial(n,k) * Eulerian(n+1,k) - n! + 1 a(n) is
 *            the triangle read by rows, offset 0
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 37, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial, factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

static seq_t **euler = NULL;      /* euler[m][k] = Eulerian(m,k)   */
static idx_t euler_max = 0;       /* number of rows currently stored */

/* Ensure Eulerian numbers are available up to row m (inclusive).  */
static void
ensure_euler_up_to(idx_t m)
{
    if (euler_max > m)
        return;      /* already have enough rows */

    /* Allocate / grow the outer array of row pointers. */
    seq_t **tmp = (seq_t **)realloc(euler, (m + 1) * sizeof(seq_t *));
    if (!tmp) abort();
    euler = tmp;

    for (idx_t i = euler_max; i <= m; ++i) {
        /* Eulerian row i has i entries (for i = 0 we have one entry). */
        idx_t len = (i == 0) ? 1 : i;
        euler[i] = (seq_t *)malloc(len * sizeof(seq_t));
        if (!euler[i]) abort();

        /* Base cases: E(i,0)=1 and E(i,i-1)=1 (for i>0). */
        euler[i][0] = (seq_t)1;
        if (i > 0)
            euler[i][len - 1] = (seq_t)1;

        /* Recurrence: E(i,k) = (k+1)E(i-1,k) + (i-k)E(i-1,k-1) */
        for (idx_t k = 1; k + 1 < len; ++k) {
            seq_t part1 = seq_mul((seq_t)(k + 1), euler[i - 1][k]);
            seq_t part2 = seq_mul((seq_t)(i - k), euler[i - 1][k - 1]);
            euler[i][k] = part1 + part2;
        }
    }

    euler_max = m + 1;
}

seq_t a(idx_t n)
{
    /* Locate the row r and column k such that index n corresponds to T(r,k). */
    idx_t r = 0;
    idx_t remaining = n;
    while (remaining >= r + 1) {
        remaining -= r + 1;
        ++r;
    }
    idx_t k = remaining;   /* 0 <= k <= r */

    /* Ensure the Eulerian numbers up to row r+1 are available. */
    ensure_euler_up_to(r + 1);

    seq_t fact_r   = factorial(r);                 /* r!            */
    seq_t binom_rk = binomial(r, k);               /* C(r,k)        */
    seq_t euler_r1k = euler[r + 1][k];             /* Eulerian(r+1,k) */

    /* T(r,k) = r! * C(r,k) * Eulerian(r+1,k) - r! + 1 */
    seq_t prod = seq_mul(seq_mul(fact_r, binom_rk), euler_r1k);
    seq_t res  = prod - fact_r + (seq_t)1;

    return res;
}
