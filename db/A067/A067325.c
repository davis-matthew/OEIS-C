/*
 * A067325  Fourth column of triangle A067323.
 * https://oeis.org/A067325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n+4) - (Catalan(n+3) + Catalan(n+2) + 2 *
 *            Catalan(n+1))
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where Catalan(k) = binom(2*k, k) / (k+1)                           */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* compute Catalan(n+1) .. Catalan(n+4) */
    seq_t c1 = binom(2*(n+1), n+1);
    c1 /= (n+2);                    /* Catalan(n+1) */

    seq_t c2 = binom(2*(n+2), n+2);
    c2 /= (n+3);                    /* Catalan(n+2) */

    seq_t c3 = binom(2*(n+3), n+3);
    c3 /= (n+4);                    /* Catalan(n+3) */

    seq_t c4 = binom(2*(n+4), n+4);
    c4 /= (n+5);                    /* Catalan(n+4) */

    /* a(n) = c4 - (c3 + c2 + 2*c1) */
    seq_t sum = c3 + c2 + ((seq_t)2 * c1);
    seq_t res = c4 - sum;
    return res;
}
