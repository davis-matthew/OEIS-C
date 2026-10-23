/*
 * A129003  a(n) = (n^3 + n^2)*3^n.
 * https://oeis.org/A129003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n^2) * 3^n = n^2 * (n + 1) * 3^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 69,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  n >= 1 (offset 1) */

#include <stdint.h>
#include "../utils/pow3.h"

/* Compute A129003: a(n) = (n^3 + n^2) * 3^n */
seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1; return 0 for safety. */
    if (n < 1) return 0;

    /* Compute 3^n once, then multiply by n^2*(n+1). */
    seq_t val = pow3(n);

    /* Convert n to seq_t once. */
    seq_t sn = (seq_t)n;

    /* Multiply sequentially; intermediate values do not exceed the final result. */
    val *= sn;           /* val = 3^n * n          */
    val *= sn;           /* val = 3^n * n^2        */
    val *= (sn + (seq_t)1); /* val = 3^n * n^2 * (n+1) */

    return val;
}
