/*
 * A164533  a(n) = sigma(sigma(n))*sigma(n).
 * https://oeis.org/A164533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(sigma(n))*sigma(n)
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1 (offset 1). */
    if (n == 0)
        return (seq_t)0;   /* behavior below offset is irrelevant */

    seq_t sigma_n   = sigma_of(n);              /* sigma(n)   */
    seq_t sigma_sig = sigma_of((idx_t)sigma_n); /* sigma(sigma(n)) */

    return sigma_n * sigma_sig;
}
