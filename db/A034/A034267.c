/*
 * A034267  a(n) = f(n,n) where f is given in A034261.
 * https://oeis.org/A034267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n+1) * (n^2 + n + 1) / (n + 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial coefficient is computed via the utility function */
/* binomial(idx_t n, idx_t k) defined in "../utils/binomial.h". */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Cast index to seq_t for arithmetic operations */
    seq_t ns = (seq_t)n;

    /* Compute binomial(2*n, n+1) */
    seq_t bin = binomial(2 * n, n + 1);

    /* Compute the numerator (n^2 + n + 1) */
    seq_t num = ns * ns + ns + 1;

    /* Multiply and then divide by (n + 2) – the division is exact */
    seq_t res = bin * num;
    res /= (ns + 2);

    return res;
}
