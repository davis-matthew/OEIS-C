/*
 * A067345  Square array read by antidiagonals:
 *          T(n,k)=(T(n,k-1)*n^2-Catalan(k-1))/(n-1) with a(n,1)=1 and
 *          a(1,k)=Catalan(k) where Catalan(k)=C(2k,k)/(k+1)=A000108(k).
 * https://oeis.org/A067345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of OEIS A067345 Read by antidiagonals the
 *            square array T(n,k) = (T(n,k-1) * n^2 - Catalan(k-1)) / (n-1),
 *            T(n,1) = 1, T(1,k) = Catalan(k), where Catalan(k) =
 *            binom(2k,k)/(k+1)
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* for size_t */
#include <stdint.h>            /* common integer types */
#include "../utils/binom.h"    /* provides seq_t binom(idx_t,idx_t) */

/* Catalan number using binom.  Works for all k≥0. */
static seq_t catalan(idx_t k)
{
    if (k == 0) return (seq_t)1;
    seq_t c = binom(2 * k, k);      /* binom returns seq_t   */
    return c / (k + 1);              /* exact division        */
}

/* Compute T(n,k) for given 1‑based indices n (row) and k (column). */
static seq_t T_value(idx_t n, idx_t k)
{
    /* Special rows/columns */
    if (n == 1) return catalan(k);
    if (k == 1) return (seq_t)1;

    seq_t cur = (seq_t)1;              /* T(n,1) */
    seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2, computed once                */

    for (idx_t j = 2; j <= k; ++j) {
        seq_t cat = catalan(j - 1);
        seq_t num = cur * nsq - cat;
        seq_t den = (seq_t)(n - 1);
        cur = num / den;               /* exact division guaranteed     */
    }
    return cur;
}

/* Main function requested by the judge */
seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 ≥ n */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;

    /* Position in the diagonal */
    idx_t offset = n - (d - 1) * d / 2;      /* 1‑based offset in this diagonal */

    /* Coordinates in T(n,k): row = offset, column = d - offset + 1 */
    idx_t row = offset;
    idx_t col = d - offset + 1;

    return T_value(row, col);
}
