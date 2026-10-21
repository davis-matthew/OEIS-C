/*
 * A067346  Square array read by antidiagonals:
 *          T(n,k)=T(n,k-1)*n^2/(n-1)-Catalan(k-1) with a(n,1)=n-1 and
 *          a(1,k)=0 where Catalan(k)=C(2k,k)/(k+1)=A000108(k).
 * https://oeis.org/A067346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by antidiagonals from the square array defined by
 *            T(n,1)=n-1, T(1,k)=0 and T(n,k)=T(n,k-1)*n^2/(n-1)-Catalan(k-1)
 *            (k>1)
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/catalan.h"  /* provides seq_t catalan(idx_t k) */

/* returns the n-th term of sequence A067346 (1‑based index) */
seq_t a(idx_t N)
{
    if (N == 0) return 0;               /* not used, just safety */

    /* find antidiagonal S such that (S-1)*S/2 >= N */
    idx_t S = 2;
    while ((S - 1) * S / 2 < N) ++S;

    /* number of elements before this diagonal */
    idx_t before = (S - 2) * (S - 1) / 2;

    /* offset within the diagonal (0‑based) */
    idx_t offset = N - before - 1;

    /* coordinates of the term: n = offset+1, k = S - offset - 1 */
    idx_t n = offset + 1;
    idx_t k = S - offset - 1;

    /* For n == 1 the array is all zeros */
    if (n == 1) return 0;

    /* compute T(n,k) iteratively */
    seq_t t = (seq_t)(n - 1);          /* T(n,1) */
    if (k == 1) return t;

    seq_t denom = (seq_t)(n - 1);
    seq_t nsq   = (seq_t)n * (seq_t)n; /* n^2 fits in native width for typical builds */

    for (idx_t kk = 2; kk <= k; ++kk) {
        /* t = t/(n-1) * n^2 - Catalan(kk-1) */
        seq_t div = t / denom;        /* exact division */
        t = div * nsq;
        t -= catalan(kk - 1);
    }
    return t;
}
