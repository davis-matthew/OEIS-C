/*
 * A023046  9th differences of factorial numbers.
 * https://oeis.org/A023046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^9 + 36*n^8 + 582*n^7 + 5460*n^6 + 32403*n^5 +
 *            124908*n^4 + 309602*n^3 + 470328*n^2 + 391641*n + 133496)*n!
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* compute the 9th‑degree polynomial via Horner’s rule */
    seq_t poly = (seq_t)1;          /* coefficient of n^9          */
    seq_t N = (seq_t)n;             /* cast index to seq_t once    */
    poly = poly * N + (seq_t)36;
    poly = poly * N + (seq_t)582;
    poly = poly * N + (seq_t)5460;
    poly = poly * N + (seq_t)32403;
    poly = poly * N + (seq_t)124908;
    poly = poly * N + (seq_t)309602;
    poly = poly * N + (seq_t)470328;
    poly = poly * N + (seq_t)391641;
    poly = poly * N + (seq_t)133496;

    /* multiply by n! */
    seq_t fact = factorial(n);
    return poly * fact;
}
