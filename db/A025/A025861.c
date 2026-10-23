/*
 * A025861  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^9)).
 * https://oeis.org/A025861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+9)^2/360 + (((2*n^2+1) mod 9) + ((n+8) mod 9) -
 *            (n mod 9) + ((n+4) mod 5) - ((n+3) mod 5))/9)
 * Range:     u8 none, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute (n+9)^2 */
    seq_t n_seq = (seq_t)n;
    seq_t sq = (n_seq + (seq_t)9) * (n_seq + (seq_t)9);

    /* Compute the small modular expression B */
    idx_t n_mod9 = n % 9;
    idx_t n_mod5 = n % 5;

    /* 2*n^2 + 1  (mod 9) */
    idx_t n_sq_mod9 = (n_mod9 * n_mod9) % 9;
    idx_t term1 = (2 * n_sq_mod9 + 1) % 9;

    /* (n+8) mod 9 */
    idx_t term2 = (n_mod9 + 8) % 9;

    /* (n+4) mod 5 */
    idx_t term4 = (n_mod5 + 4) % 5;

    /* (n+3) mod 5 */
    idx_t term5 = (n_mod5 + 3) % 5;

    /* B = term1 + term2 - n_mod9 + term4 - term5 */
    int64_t B = (int64_t)term1 + (int64_t)term2 - (int64_t)n_mod9
                + (int64_t)term4 - (int64_t)term5;

    /* numerator = sq + 40*B   (B may be negative) */
    seq_t numerator;
    if (B >= 0)
    {
        numerator = sq + (seq_t)((uint64_t)40 * (uint64_t)B);
    }
    else
    {
        /* B is negative; subtract the absolute value */
        numerator = sq - (seq_t)((uint64_t)40 * ((uint64_t)(-B)));
    }

    /* a(n) = floor(numerator / 360) */
    return numerator / (seq_t)360;
}
