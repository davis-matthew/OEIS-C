/*
 * A116871  Sixth column of triangle A067323.
 * https://oeis.org/A116871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n+6) - (14·Catalan(n+1) + 5·Catalan(n+2) +
 *            2·Catalan(n+3) + 1·Catalan(n+4) + 1·Catalan(n+5))
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"   /* provides seq_t catalan(idx_t k); */

seq_t a(idx_t n)
{
    /* indices for Catalan numbers needed */
    idx_t n1 = n + 1;
    idx_t n2 = n + 2;
    idx_t n3 = n + 3;
    idx_t n4 = n + 4;
    idx_t n5 = n + 5;
    idx_t n6 = n + 6;

    /* compute Catalan numbers */
    seq_t c1 = catalan(n1);
    seq_t c2 = catalan(n2);
    seq_t c3 = catalan(n3);
    seq_t c4 = catalan(n4);
    seq_t c5 = catalan(n5);
    seq_t c6 = catalan(n6);

    /* combine according to the formula */
    seq_t sub = 14 * c1 + 5 * c2 + 2 * c3 + c4 + c5;
    return c6 - sub;
}
