/*
 * A028672  Pseudo Galois numbers for d=10.
 * https://oeis.org/A028672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n * Product_{k=0..n-1} (10^n - 10^k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n once */
    seq_t ten_n = pow10(n);

    /* Start result with the leading 10^n factor */
    seq_t result = ten_n;

    /* Compute successive powers of 10 (10^k) incrementally to avoid
       repeated exponentiation calls. */
    seq_t ten_k = 1;   /* 10^0 */

    for (idx_t k = 0; k < n; ++k)
    {
        /* term = 10^n - 10^k */
        seq_t term = ten_n - ten_k;

        /* Multiply into the running product */
        result = result * term;

        /* Advance 10^k to 10^(k+1) for next loop iteration */
        ten_k = ten_k * 10;
    }

    return result;
}
