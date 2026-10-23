/*
 * A144383  T(n,k) = [t^k] 1/(t^n + t + 1), square array read by ascending
 *          antidiagonals (n >= 1, k >= 0).
 * https://oeis.org/A144383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 44, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   coeff_nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* T(n,k) = coefficient of t^k in 1/(t^n + t + 1).   
   The sequence a(N) is read from this square array by ascending
   antidiagonals, i.e. (n,k) is the unique pair with n ≥ 1, k ≥ 0
   such that the (n,k)-entry appears at position N in that ordering. */
#include <stdlib.h>
#include "../utils/coeff_nk.h"

/* public function: a(N) (1‑based index) */
seq_t a(idx_t N)
{
    if (N < 1)            /* offset starts at 1; outside range returns 0 */
        return (seq_t)0;

    /* find the diagonal d such that d(d+1)/2 < N ≤ (d+1)(d+2)/2   */
    idx_t d = 0;          /* diagonal index, 0‑based */
    idx_t cumulative = 1; /* terms up to diagonal d (d=0 gives 1 term) */
    while (cumulative < N)
    {
        ++d;
        cumulative += d + 1;   /* size of diagonal d */
    }

    /* number of terms before this diagonal */
    idx_t S_prev = d * (d + 1) / 2;

    /* position on current diagonal, 0‑based */
    idx_t p = N - S_prev - 1;

    /* map to array coordinates (n,k) */
    seq_t n = (seq_t)(d + 1 - p);  /* n ≥ 1 */
    seq_t k = (seq_t)p;           /* k ≥ 0 */

    return coeff_nk(n, k);
}
