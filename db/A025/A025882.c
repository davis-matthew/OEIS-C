/*
 * A025882  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A025882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2+20*n)/560 + ((2*n^2+3) mod 5)/5 +
 *            ((2*n^2+5*n+4) mod 7)/7)
 * Range:     u8 none, u16 n <= 273, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t part1 = nn * (nn + (seq_t)20);                           /* n^2+20*n   */
    seq_t rem5  = ((seq_t)2 * nn * nn + (seq_t)3) % (seq_t)5;      /* (2*n^2+3) mod 5 */
    seq_t rem7  = ((seq_t)2 * nn * nn + (seq_t)5 * nn + (seq_t)4) % (seq_t)7; /* (2*n^2+5*n+4) mod 7 */

    seq_t numerator = part1                       /* /560 term   */
                      + rem5 * (seq_t)112         /* (rem5)/5   */
                      + rem7 * (seq_t)80;         /* (rem7)/7   */

    return numerator / (seq_t)560;
}
