/*
 * A110430  Arithmetic mean of all n-digit positive even numbers.
 * https://oeis.org/A110430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1/2 * (10^(n-1) + 10^n - 2) for n >= 1, a(0) = 5. (Here n
 *            is the OEIS index with offset 0.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)5;

    /* Compute 10^n and 10^(n+1) using the utility function pow10. */
    seq_t pow10_n     = pow10(n);        /* 10^n   */
    seq_t pow10_n_plus1 = pow10(n + 1); /* 10^(n+1) */

    /* Apply the formula: (10^(n-1) + 10^n - 2) / 2.
       Here 10^(n-1) = 10^n / 10 = pow10_n / 10,
       but we keep the formula as given for clarity. */
    seq_t sum = pow10_n + pow10_n_plus1 - (seq_t)2;
    return sum / (seq_t)2;
}
