/*
 * A066971  a(n) = sigma(sigma(sigma(n))).
 * https://oeis.org/A066971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(sigma(sigma(n)))
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    /* compute sigma(n) */
    seq_t s1 = sigma_of(n);
    /* compute sigma(sigma(n)) */
    seq_t s2 = sigma_of((idx_t)s1);
    /* compute sigma(sigma(sigma(n))) */
    return sigma_of((idx_t)s2);
}
