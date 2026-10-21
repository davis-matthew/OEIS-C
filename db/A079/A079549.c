/*
 * A079549  Array T(m,n) = m*phi(n) - phi(mn) (m,n >= 1), read by
 *          antidiagonals.
 * https://oeis.org/A079549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n') = m*phi(n') - phi(m*n'), where (m,n') are the
 *            coordinates of the nth term read by antidiagonals of the array
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Locate the diagonal containing the nth term.
       The number of terms up to and including diagonal s is (s-1)*s/2. */
    idx_t cumulative = 0;   /* terms before the current diagonal */
    idx_t s = 2;            /* diagonal index (sum of row + column) */

    while (cumulative + (s - 1) < n) {
        cumulative += (s - 1);
        ++s;
    }

    /* Offset of the term within the current diagonal (0‑based). */
    idx_t offset = n - cumulative - 1;

    /* Coordinates in the array: m decreases from s-1 to 1,
       n' increases from 1 upwards. */
    idx_t m = (s - 1) - offset;
    idx_t n_prime = offset + 1;

    /* Compute the value using the totient function from utils/phi.h. */
    seq_t phi_n    = phi((idx_t)n_prime);          /* phi(n')   */
    idx_t prod     = m * n_prime;                 /* m*n'      */
    seq_t phi_mn   = phi(prod);                   /* phi(m*n') */
    seq_t result   = (seq_t)m * phi_n - phi_mn;   /* T(m,n')   */

    return result;
}
