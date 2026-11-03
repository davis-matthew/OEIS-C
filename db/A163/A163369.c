/*
 * A163369  a(n) = sigma(sigma(phi(n))).
 * https://oeis.org/A163369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(sigma(phi(n)))
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 993 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A163369 */

/* Include the utility functions for phi and sigma */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Compute φ(n) */
    seq_t phi = totient_seq((seq_t)n);

    /* Compute σ(φ(n)) */
    idx_t phi_int = (idx_t)phi;          /* safe cast for n up to 2^64-1 */
    seq_t sigma_phi = sigma_of(phi_int);

    /* Compute σ(σ(φ(n))) */
    idx_t sigma_phi_int = (idx_t)sigma_phi; /* safe cast */
    seq_t result = sigma_of(sigma_phi_int);

    return result;
}
