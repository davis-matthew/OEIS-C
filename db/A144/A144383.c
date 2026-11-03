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
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* T(n,k) = coefficient of t^k in 1/(t^n + t + 1).   
   The sequence a(N) is read from this square array by ascending
   antidiagonals, i.e. (n,k) is the unique pair with n ≥ 1, k ≥ 0
   such that the (n,k)-entry appears at position N in that ordering. */
#include <stdlib.h>
#include "../utils/mem.h"

/* compute coefficient T(n,k) for n≥1, k≥0 */
static seq_t coeff_nk(seq_t n, seq_t k)
{
    if (k == 0)           /* the constant term is always 1 */
        return (seq_t)1;
    if (k < 0 || n < 1)
        return (seq_t)0;  /* defensive, should not happen */

    /* allocate array of k+1 coefficients */
    seq_t *coeff = mem_new(seq_t, (size_t)k + 1);
    if (!coeff)           /* allocation failure – return 0 */
        return (seq_t)0;

    coeff[0] = (seq_t)1;
    for (seq_t j = 1; j <= k; ++j)
    {
        seq_t c = -coeff[(size_t)(j - 1)];
        if (j >= n)
            c -= coeff[(size_t)(j - n)];
        coeff[(size_t)j] = c;
    }
    seq_t result = coeff[(size_t)k];
    mem_free(coeff);
    return result;
}

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
