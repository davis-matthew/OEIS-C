/*
 * A063732  Numbers whose Lucas representation excludes L_0 = 2.
 * https://oeis.org/A063732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - floor(n*phi) - 2 (phi = (1+√5)/2)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses Fibonacci convergent  F51/F50 for pi ≈ φ to compute floor(n*phi) exactly
   for all n that fit in 64‑bit indices. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fibonacci convergent for φ:
       p/q = F51/F50 = 20365011074 / 12586269025  ≈ 1.6180339887498948482 */
    const unsigned long long PHI_NUM = 20365011074ULL;
    const unsigned long long PHI_DEN = 12586269025ULL;

    seq_t n_seq = (seq_t)n;

    /* Compute floor(n*phi) = floor( n * PHI_NUM / PHI_DEN ) */
    __uint128_t prod = (__uint128_t)n_seq * PHI_NUM;
    seq_t floor_n_phi = (seq_t)(prod / PHI_DEN);

    /* a(n) = 3*n - floor(n*phi) - 2 */
    return 3 * n_seq - floor_n_phi - 2;
}
