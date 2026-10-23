/*
 * A187738  G.f.: Sum_{n>=0} (3*n+1)^n * x^n / (1 + (3*n+1)*x)^n.
 * https://oeis.org/A187738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187738
   Formula: a(n) = (3*n+5) * 3^(n-1) * n! / 2   for n > 0,
   a(0) = 1
   (The constant term 1 is printed as a(0) = 1 in the OEIS file.) */

#include <stdint.h>   /* for uint64_t (idx_t) and implementation‑defined seq_t */

/* Generic factorial and power computation using the arithmetic operations
   available on whatever type seq_t happens to be.  This works for ordinary
   integer types as well as arbitrary‑precision types that overload the usual
   operators.  The loops are modest (≤ 2·n multiplications for n ≤ 10 000) and
   therefore comfortably fit within the 2‑second per call requirement. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) = 1 by definition */
        return (seq_t)1;
    }

    /* Compute n!  (factorial) */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute 3^(n-1) */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = (idx_t)1; i < n; ++i) {
        pow3 *= (seq_t)3;
    }

    /* Compute (3*n+5) */
    idx_t coeff_idx = (idx_t)3 * n + (idx_t)5;   /* stays in idx_t */
    seq_t coeff = (seq_t)coeff_idx;

    /* Combine everything and divide by 2 (exact) */
    seq_t result = coeff * pow3 * fact;
    result /= (seq_t)2;

    return result;
}
