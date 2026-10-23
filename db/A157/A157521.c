/*
 * A157521  Triangle T(n, k) = n! * binomial(n, k)*( psi(n-k+1) - psi(k+1) ),
 *          read by rows.
 * https://oeis.org/A157521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = flattening of the triangle T(r, k) * T(r, k) = r! *
 *            binomial(r, k) * ( H_{r-k} - H_k ), * where H_m = 1 + 1/2 + ...
 *            + 1/m. * The sequence is read row by row, starting with a(0)=0.
 *            * For n >= 0 the function returns the nth term
 * Range:     i8 n <= 14, i16 n <= 28, i32 n <= 67, i64 n <= 174,
 *            i128 n <= 470, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes T(r,k) for 0 <= k <= r.  Assumes r is small enough that
 * factorials and binomials fit into seq_t.  Uses the identity
 *   T(r,k) = sum_{i=k+1}^{r-k} ( r! * binomial(r,k) ) / i   (if k <= r/2, otherwise
 *   T(r,k) = -sum_{i=r-k+1}^{k} ( r! * binomial(r,k) ) / i ).
 *
 * The division is exact because the numerator is divisible by every i in the sum.
 */
static seq_t compute_T(idx_t r, idx_t k)
{
    /* Handle trivial row */
    if (r == 0) {
        return 0;
    }

    /* If k > r/2, we will use symmetry outside and any k passed here
     * is guaranteed to satisfy k <= r/2.  If this function is called
     * directly with k > r/2, it will still compute correctly because the
     * recurrence does not assume symmetry. */

    /* Compute factorial r! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= r; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute binomial coefficient C(r,k) */
    seq_t bin = 1;
    for (idx_t i = 1; i <= k; ++i) {
        bin *= (seq_t)(r - k + i);
        bin /= (seq_t)i;          /* exact division */
    }

    /* B = r! * C(r,k) */
    seq_t B = fact * bin;

    /* If k == r/2 and r even, T(r,k) = 0 */
    if ((r % 2) == 0 && k == r / 2) {
        return 0;
    }

    /* Sum of integer terms */
    seq_t sum = 0;
    for (idx_t i = k + 1; i <= r - k; ++i) {
        sum += B / (seq_t)i;      /* division is exact */
    }

    return sum;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }

    /* Column index within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Use symmetry: T(r, r-k) = - T(r, k) when k > r/2. */
    if (k > r / 2) {
        return -compute_T(r, r - k);
    } else {
        return compute_T(r, k);
    }
}
