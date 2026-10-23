/*
 * A181965  a(n) = 10^(2*n+1) - 10^n - 1.
 * https://oeis.org/A181965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 10^(2*n+1) - 10^n - 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A181965: number with n 9's, an 8, then n 9's. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^(2*n+1) and 10^n using the provided pow10 helper. */
    idx_t exp1 = 2 * n + 1;
    seq_t high = pow10(exp1);   /* 10^(2*n+1) */
    seq_t mid  = pow10(n);      /* 10^n      */

    /* a(n) = high - mid - 1; the subtraction order guarantees non‑negative
       intermediate results for all n >= 0. */
    seq_t result = high - mid;
    result = result - (seq_t)1;
    return result;
}
