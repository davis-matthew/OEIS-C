/*
 * A212805  a(n) = (1-(n-1)/2^n)*(n!)^n.
 * https://oeis.org/A212805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1-(n-1)/2^n)*(n!)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   ipow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/pow2.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Computes the OEIS sequence A212805 */
seq_t a(idx_t n)
{
    /* Handle the offset n = 0 explicitly to avoid unsigned underflow */
    if (n == 0)
        return (seq_t)2;

    /* 2^n as a seq_t (big integer if necessary) */
    seq_t pow2n = pow2(n);

    /* Numerator of the rational factor: 2^n - (n-1) */
    seq_t two_n_minus = pow2n - (seq_t)(n - 1);

    /* n! as a seq_t */
    seq_t fact = factorial(n);

    /* (n!)^n */
    seq_t fact_pow = ipow(fact, (seq_t)n);

    /* Combine numerator and divisor */
    seq_t numerator_times = fact_pow * two_n_minus;   /* big integer multiply */
    seq_t result = numerator_times / pow2n;           /* integer division */

    return result;
}
