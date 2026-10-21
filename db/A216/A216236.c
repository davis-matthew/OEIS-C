/*
 * A216236  Square array T, read by antidiagonals: T(n,k) = 0 if n-k>=4 or if
 *          k-n>=5, T(3,0) = T(2,0) = T(1,0) = T(0,0) = T(0,1) = T(0,2) =
 *          T(0,3) = T(0,4) = 1, T(n,k) = T(n-1,k) + T(n,k-1).
 * https://oeis.org/A216236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the antidiagonal enumeration of the square array T
 *            defined by: T(n,k) = 0 if n-k >= 4 or k-n >= 5 T(0,0..4) = 1
 *            T(1..3,0) = 1 T(n,k) = T(n-1,k) + T(n,k-1) otherwise. a(n) =
 *            T(i,j) where (i,j) is the n‑th element read by diagonals (i+j) =
 *            d with 0 ≤ i ≤ d, j = d - i
 * Range:     u8 n <= 69, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_D 500   /* sufficient for n ≤ 10000 (d ≈ 141) */

static seq_t T[MAX_D][MAX_D];
static int T_initialized = 0;

static void init_T(void)
{
    if (T_initialized) return;
    for (size_t n = 0; n < MAX_D; ++n)
    {
        for (size_t k = 0; k < MAX_D; ++k)
        {
            /* base values */
            if (n == 0 && k <= 4) { T[n][k] = (seq_t)1; continue; }
            if (n <= 3 && k == 0) { T[n][k] = (seq_t)1; continue; }

            /* band condition */
            if (n >= k + 4 || k >= n + 5) { T[n][k] = (seq_t)0; continue; }

            /* recurrence */
            seq_t a = (seq_t)0, b = (seq_t)0;
            if (n > 0) a = T[n - 1][k];
            if (k > 0) b = T[n][k - 1];
            T[n][k] = a + b;
        }
    }
    T_initialized = 1;
}

/* compute the nth term of the sequence */
seq_t a(idx_t n)
{
    init_T();           /* ensure the table is ready */

    /* find diagonal d such that d(d+1)/2 ≤ n < (d+1)(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) d++;

    idx_t prev_total = d * (d + 1) / 2;   /* total terms before this diagonal */
    idx_t pos = n - prev_total;          /* 0‑based position on the diagonal */
    idx_t i = pos;                       /* row index */
    idx_t j = d - pos;                   /* column index */

    /* bounds guard – should never be triggered for the problem limits */
    if (i >= MAX_D || j >= MAX_D) return (seq_t)0;

    return T[i][j];
}
