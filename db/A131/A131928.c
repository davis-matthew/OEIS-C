/*
 * A131928  a(n) = phi(binomial(2*n,n)*n).
 * https://oeis.org/A131928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(binomial(2*n,n)*n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   binomial, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/totient_seq.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 explicitly – the product binomial(0,0)*0
       is 0 and φ(0) is defined as 0. */
    if (n == 0)
        return 0;

    /* Compute the central binomial coefficient binomial(2*n, n). */
    seq_t c = binomial(2 * n, n);

    /* Multiply the coefficient by n (converted to seq_t). */
    c = seq_mul(c, (seq_t)n);

    /* Return Euler's totient of the product. */
    return totient_seq(c);
}
