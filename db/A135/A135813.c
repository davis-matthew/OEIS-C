/*
 * A135813  Number of coincidence-free length n lists of 7-tuples with all
 *          numbers 1..n in tuple position k, for k=1..7.
 * https://oeis.org/A135813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..n} (-1)^(n-j) * binomial(n,j) * (j!)^7
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t pos = 0;      /* accumulate terms with (-1)^(n-j) == +1   */
    seq_t neg = 0;      /* accumulate terms with (-1)^(n-j) == -1   */
    seq_t binom = 1;    /* binomial(n,0) */
    seq_t f7    = 1;    /* (0!)^7 = 1 */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = binom * f7;

        /* sign depends on parity of (n - j) */
        if ( ((n - j) & 1) == 0 )
            pos += term;
        else
            neg += term;

        if (j == n) break;   /* completed the last term */

        /* prepare for next iteration: compute (j+1)!^7 */
        idx_t jp = j + 1;
        seq_t pow_j = (seq_t)jp;
        for (int k = 1; k < 7; ++k)
            pow_j *= (seq_t)jp;
        f7 *= pow_j;

        /* update binomial coefficient to C(n, j+1) */
        binom = (binom * (seq_t)(n - j)) / (seq_t)jp;
    }

    /* the final value is positive (by definition of the sequence) */
    return pos - neg;
}
