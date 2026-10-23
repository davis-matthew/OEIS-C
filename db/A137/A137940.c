/*
 * A137940  Triangle read by rows, antidiagonals of an array formed by A000012
 *          * A001263 (transform).
 * https://oeis.org/A137940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137940: a(n) = sum_{t=0}^{min(i,j)} N[j][t]
   where i = s - j,   j = k - 1
         k = n - s(s+1)/2
         s = min{s | (s+1)(s+2)/2 >= n}
   and N[n][k] are Narayana numbers (0‑based):
   N[n][k] = binom(n+1, k+1) * binom(n+1, k) / (n+1).  */
#include "../utils/binom.h"

static inline seq_t narayana(idx_t n, idx_t k)
{
    seq_t a = binom(n+1, k+1);      /* C(n+1, k+1) */
    seq_t b = binom(n+1, k);        /* C(n+1, k)   */
    return (a * b) / (n+1);         /* exact division */
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal s (0‑based) that contains position n */
    idx_t s = 0;
    while ((s+1)*(s+2)/2 < n) ++s;

    /* Previous number of terms before that diagonal */
    idx_t prev = s*(s+1)/2;

    /* Position inside the diagonal (1‑based) */
    idx_t k = n - prev;

    /* Convert to indices j (column) and i (row) */
    idx_t j = k-1;
    idx_t i = s - j;
    idx_t m = (i < j) ? i : j;   /* limit for summation */

    /* Sum Narayana numbers up to m */
    seq_t sum = 0;
    for (idx_t t = 0; t <= m; ++t)
        sum += narayana(j, t);

    return sum;
}
