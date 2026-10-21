/*
 * A218277  Convolution of level 3 of the divisor function.
 * https://oeis.org/A218277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/72)*(31*sigma3(n) - sigma3(3*n) + 7*sigma(n) -
 *            sigma(3*n) - 30*n*sigma(n) + 6*n*sigma(3*n))
 * Range:     u8 n <= 17, u16 n <= 107, u32 n <= 4031, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* for sqrt on double */
#include "../utils/sigma_k_uint128.h"

/* External linkage function implementing the sequence */
seq_t a(idx_t n)
{
    /* σ(·) and σ₃(·) needed for n and 3n */
    unsigned __int128 sig_n   = sigma_k_uint128(n, 1);
    unsigned __int128 sig3_n  = sigma_k_uint128(n, 3);
    unsigned __int128 sig_3n  = sigma_k_uint128(3 * n, 1);
    unsigned __int128 sig3_3n = sigma_k_uint128(3 * n, 3);

    unsigned __int128 n_val = n;

    unsigned __int128 term =
          (31 * sig3_n)
        - sig3_3n
        + (7 * sig_n)
        - sig_3n
        - (30 * n_val * sig_n)
        + (6 * n_val * sig_3n);

    term /= 72;

    return (seq_t)term;
}
