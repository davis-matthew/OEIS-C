/*
 * A215001  a(n) = 1 + floor(e^(1 + 1/2 + 1/3 + ... + 1/n)).
 * https://oeis.org/A215001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + floor( exp( 1 + 1/2 + 1/3 + ... + 1/n ) )
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code evaluates the harmonic sum as a long double and then
   searches for the smallest integer k whose natural logarithm
   exceeds this sum.  The result fits comfortably in any unsigned
   integer type used for seq_t. */

#include <math.h>      /* for logl, expl (used only on temporary long doubles) */

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1; 0 is safe */

    /* Compute the harmonic sum H_n in high‑precision long double */
    long double H = 0.0L;
    for (idx_t i = 1; i <= n; ++i)
        H += 1.0L / (long double)i;

    /* Find the least integer k with log(k) > H */
    idx_t k = 1;
    while (logl((long double)k) <= H)
        ++k;

    return (seq_t)k;   /* k = floor(exp(H)) + 1 */
}
