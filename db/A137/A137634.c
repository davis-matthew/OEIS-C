/*
 * A137634  Square array where T(n,k) = Sum_{j=0..k} C(n+2*j,j)*C(n+2*j,k-j),
 *          read by antidiagonals.
 * https://oeis.org/A137634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 42, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A137634
 *
 *  a(n) is the n‑th element read by antidiagonals of the square array
 *  T(i,j) with
 *
 *      T(i,j) = Σ_{k=0..j} C(i+2k , k) * C(i+2k , j-k)
 *
 *  where the binomial coefficient is taken in the usual integer sense.
 *  The index n starts at 0.  The function below obtains the
 *  coordinates (i,j) of the n‑th antidiagonal entry and evaluates the
 *  defining sum by a multiplicative binomial routine.
 */

#include <stddef.h>   /* for size_t if needed */

/* ------------------------------------------------------------------ */
/* Multiplicative binomial coefficient
 *
 *   binom(n,k) = n! / (k! * (n-k)!)
 *   the routine reduces k to min(k,n-k) for efficiency.
 *   It assumes that intermediate results fit into seq_t.
 */
static seq_t binom_val(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;          /* use symmetry */

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide; division is exact */
        result = result * (n - k + i);
        result = result / i;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    /* find the diagonal index s such that (s+1)(s+2)/2 > n   */
    idx_t s = 0;
    while ((s + 1ULL) * (s + 2ULL) / 2ULL <= n) {
        ++s;
    }

    /* number of entries before diagonal s */
    idx_t prev = s * (s + 1) / 2;
    idx_t t = n - prev;          /* offset within diagonal s   */
    idx_t i = s - t;             /* row coordinate  (i >= 0)  */
    idx_t j = t;                 /* column coordinate (j >= 0)*/

    /* compute T(i,j) = Σ_{k=0..j} C(i+2k , k) * C(i+2k , j-k) */
    seq_t total = 0;
    for (idx_t k = 0; k <= j; ++k) {
        seq_t c1 = binom_val(i + 2 * k, k);
        seq_t c2 = binom_val(i + 2 * k, j - k);
        total += c1 * c2;
    }

    return total;
}
