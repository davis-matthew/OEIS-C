/*
 * A162462  Sum of all numbers from n to sigma(n).
 * https://oeis.org/A162462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n+1) + ... + sigma(n) = T(sigma(n)) - T(n-1), where
 *            T(x) = x*(x+1)/2
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* compute sigma(n) */
    seq_t sigma_seq = sigma_of(n);          /* sigma(n) as seq_t */
    idx_t sigma      = (idx_t)sigma_seq;    /* promote to idx_t for safe arithmetic */

    /* triangular number T(sigma(n)) */
    idx_t t_sigma   = sigma * (sigma + 1) / 2;

    /* triangular number T(n-1) */
    idx_t n_minus1  = n - 1;
    idx_t t_nm1     = n_minus1 * (n_minus1 + 1) / 2;

    /* result as seq_t */
    idx_t result    = t_sigma - t_nm1;
    return (seq_t)result;
}
