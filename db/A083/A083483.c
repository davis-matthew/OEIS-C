/*
 * A083483  Number of forests with two connected components in the complete
 *          graph K_{n}.
 * https://oeis.org/A083483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-4)*(n-1)*(n+6)/2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_t.h"
/* a(1)=0, a(2)=1, a(3)=3, a(4)=15, a(5)=110, ... */

seq_t a(idx_t n)
{
    /* Handle small n explicitly to avoid negative exponents */
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 3;

    /* For n >= 4, exponent is non‑negative */
    seq_t coeff = (seq_t)(n - 1) * (seq_t)(n + 6);
    coeff /= 2;                      /* (n-1)*(n+6)/2  is an integer */

    seq_t pow = pow_seq_t((seq_t)n, n - 4);  /* n^(n-4) */
    return coeff * pow;
}
