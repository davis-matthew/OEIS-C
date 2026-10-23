/*
 * A026044  a(n) = (d(n)-r(n))/2, where d = A026043 and r is the periodic
 *          sequence with fundamental period (1,1,0,0).
 * https://oeis.org/A026044
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( n * ( 2*(n-5)^2 + 11*(n-5) + 54 ) - s ) / 12 where s =
 *            6 if (n-5) mod 4 is 0 or 1, otherwise s = 0. This is equivalent
 *            to the OEIS formula for A026044 with offset 5
 * Range:     u8 none, u16 n <= 33, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned subtract_table[4] = { 6, 6, 0, 0 };

seq_t a(idx_t n)
{
    /* k = n - 5 ; 4‑periodic subtraction depends only on k mod 4  */
    seq_t k = (seq_t)(n) - (seq_t)5;            /* underflow OK for n<5 */

    /* term1 is simply n itself   */
    seq_t term1 = (seq_t)n;

    /* term2 = 2*k^2 + 11*k + 54   */
    seq_t term2 = 2 * k * k + 11 * k + (seq_t)54;

    /* product of the two terms   */
    seq_t prod = term1 * term2;

    /* periodic subtraction  */
    unsigned sub = subtract_table[(unsigned)(k & 3)];

    /* final division by 12   */
    return (prod - sub) / (seq_t)12;
}
