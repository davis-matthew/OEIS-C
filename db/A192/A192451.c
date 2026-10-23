/*
 * A192451  Number of primes between successive hexagonal numbers.
 * https://oeis.org/A192451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes strictly between the (n−1)th and nth
 *            hexagonal numbers H(n) = n*(2n-1)
 * Range:     u8 n <= 11, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Compute H(n) = n*(2*n-1) in seq_t arithmetic */
    seq_t n_sext = (seq_t)n;
    seq_t hex_n   = n_sext * ((seq_t)2 * n_sext - 1);

    /* Compute H(n-1).  For n=1 the value is 0 and
       we avoid using unsigned wraparound. */
    seq_t hex_prev = 0;
    if (n > 1) {
        idx_t nm1 = n - 1;
        seq_t nm1_sext = (seq_t)nm1;
        hex_prev = nm1_sext * ((seq_t)2 * nm1_sext - 1);
    }

    /* Primes strictly between H(n-1) and H(n) are those ≤ H(n)-1
       minus those ≤ H(n-1).  For n=1 this correctly yields 0. */
    idx_t top = (idx_t)(hex_n - 1);            /* hex_n is ≥1 so no underflow */
    seq_t pi_top  = compute_pi(top);
    seq_t pi_prev = compute_pi((idx_t)hex_prev);

    return pi_top - pi_prev;
}
