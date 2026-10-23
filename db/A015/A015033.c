/*
 * A015033  q-Catalan numbers (binomial version) for q=3.
 * https://oeis.org/A015033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((q-1)/(q^(n+1)-1)) * Π_{k=0}^{n-1}
 *            (q^(2n-k)-1)/(q^(k+1)-1) with q=3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Handle the offset zero explicitly */
    if (n == 0)
        return 1;

    const seq_t q = 3;            /* fixed q for this sequence */

    /* Pre‑compute needed powers of 3:
     *    3^(2n)   for the product loop
     *    3^(n+1)   for the final division */
    idx_t two_n    = n + n;
    idx_t n_plus_1 = n + 1;

    seq_t big_pow = 1;               /* will become 3^(2n) */
    for (idx_t i = 0; i < two_n; ++i)
        big_pow *= q;

    seq_t pow_n_plus_1 = 1;          /* will become 3^(n+1) */
    for (idx_t i = 0; i < n_plus_1; ++i)
        pow_n_plus_1 *= q;

    /* Compute the product part of the formula */
    seq_t product   = 1;
    seq_t pow_small = 1;                /* 3^0 initially */

    for (idx_t k = 0; k < n; ++k) {
        pow_small *= q;                 /* now 3^(k+1) */

        /* (q^(2n-k) - 1) and (q^(k+1) - 1) are all positive */
        seq_t numerator   = big_pow - 1;
        seq_t denominator = pow_small - 1;

        /* Cancel common factors with the current product */
        seq_t g1 = seq_gcd(product, denominator);
        product /= g1;
        denominator /= g1;

        /* Cancel any remaining common factor between numerator and denominator */
        seq_t g2 = seq_gcd(numerator, denominator);
        numerator /= g2;
        denominator /= g2;

        /* Multiply the reduced numerator and divide by any leftover denominator */
        product *= numerator;
        if (denominator != 1)
            product /= denominator;

        /* Prepare next numerator term: 3^(2n-(k+1)) */
        big_pow /= q;
    }

    /* Final multiplication by (q-1) and division by (q^(n+1)-1) */
    seq_t top    = q - 1;                  /* = 2 */
    seq_t bottom = pow_n_plus_1 - 1;       /* = 3^(n+1) - 1 */

    product *= top;

    /* Simplify the final fraction */
    seq_t g3 = seq_gcd(product, bottom);
    product /= g3;
    bottom /= g3;

    return product / bottom;
}
