/*
 * A117794  Hexagonal numbers divisible by 6.
 * https://oeis.org/A117794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (3 * ((6*n*(2*n-5)+19) + ((n%2?4*n-5:5-4*n)))) / 2
 * This implements the explicit expression given in OEIS A117794:
 *   a(n) = 3/2 * (-1)^n * (-4 n + (-1)^n * (6*n*(2*n-5)+19) + 5)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* compute 6*n*(2*n-5)+19 */
    seq_t term = (seq_t)6 * n_seq * (((seq_t)2 * n_seq) - (seq_t)5);
    term += (seq_t)19;

    /* add the parity-dependent part: 4*n-5 for odd n, 5-4*n for even n */
    seq_t parity;
    if (n & (idx_t)1)        /* n odd */
        parity = (seq_t)4 * n_seq - (seq_t)5;
    else                    /* n even */
        parity = (seq_t)5 - (seq_t)4 * n_seq;

    /* combine, multiply by 3, then divide by 2 */
    seq_t numerator = (term + parity) * (seq_t)3;
    return numerator / (seq_t)2;
}
