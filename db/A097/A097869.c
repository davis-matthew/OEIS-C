/*
 * A097869  Expansion of g.f.: (1+x^4+x^5+x^9)/((1-x)*(1-x^2)*(1-x^4)^2).
 * https://oeis.org/A097869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/48 * [ 30 + 18*(-1)^(n/2) + 64*(n/2) + 12*(n/2)^2 +
 *            8*(n/2)^3] for even n * a(n) = 1/48 * [ (36 + 12*(-1)^(n/2) +
 *            16*(n/2) + 8*(n/2)^2) * (1 + n/2) ] for odd n * The sequences
 *            are computed directly from these closed forms
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use half index k = floor(n/2) */
    idx_t k = n >> 1;           /* n/2 */

    if (n & 1u) { /* odd n = 2*k + 1 */
        /* term = 36 + 12*(-1)^k + 16*k + 8*k^2 */
        seq_t term = 36 + 16 * (seq_t)k + 8 * (seq_t)k * (seq_t)k;
        if (k & 1u)          /* (-1)^k = -1 when k odd */
            term -= 12;      /* subtract 12 */
        else
            term += 12;      /* add 12 */

        /* Multiply by (1 + k) */
        term = term * ((seq_t)k + 1u);

        /* Final division by 48 */
        return term / 48;
    }

    /* even n = 2*k */
    /* term = 30 + 18*(-1)^k + 64*k + 12*k^2 + 8*k^3 */
    seq_t term = 30 + 64 * (seq_t)k + 12 * (seq_t)k * (seq_t)k + 8 * (seq_t)k * (seq_t)k * (seq_t)k;
    if (k & 1u)          /* (-1)^k = -1 when k odd */
        term -= 18;      /* subtract 18 */
    else
        term += 18;      /* add 18 */

    /* Final division by 48 */
    return term / 48;
}
