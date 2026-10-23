/*
 * A047885  Array a(m,n) = pi(m+n) - pi(m) - pi(n) read by antidiagonals,
 *          where pi() = A000720 (m,n >= 0).
 * https://oeis.org/A047885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = pi(m+n) - pi(m) - pi(n) read by antidiagonals, where
 *            (m,n) are obtained from the index n by antidiagonal coordinates
 *            (m = offset in diagonal, n = sum - offset)
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_pi.h"

/* Return the diagonal index s such that the entries up to sum s
   contain the term at position n.  */
static idx_t antidiagonal_sum(idx_t n)
{
    idx_t s = 0;
    idx_t count = 1;                     /* number of entries up to diagonal s */
    while (count <= n) {
        ++s;
        count += s + 1;                  /* add size of next diagonal (s+1) */
    }
    return s;
}

seq_t a(idx_t n)
{
    /* Find the diagonal (sum) for this index */
    idx_t s = antidiagonal_sum(n);

    /* Compute offset within the diagonal */
    idx_t prev_total = s * (s + 1) / 2;  /* entries up to previous diagonal */
    idx_t offset = n - prev_total;

    /* Antidiagonal coordinates (m, k) */
    idx_t m = offset;
    idx_t k = s - offset;

    /* Compute the pi values and apply the formula */
    seq_t pi_s = compute_pi(s);
    seq_t pi_m = compute_pi(m);
    seq_t pi_k = compute_pi(k);

    return pi_s - pi_m - pi_k;
}
