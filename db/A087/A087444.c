/*
 * A087444  Numbers that are congruent to {1, 4} mod 9.
 * https://oeis.org/A087444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (18*n - 17 - 3*(-1)^n)/4
 * Range:     u8 n <= 28, u16 n <= 8163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent rewriting to avoid overflow: */
/* For odd n: a(n) = (9*n - 7)/2 */
/* For even n: a(n) = (9*n - 10)/2 */

seq_t a(idx_t n)
{
    /* compute 9*n in seq_t */
    seq_t nine_n = (seq_t)n * 9;

    if (n & 1)                /* odd n */
        return (nine_n - 7) / 2;
    else                      /* even n */
        return (nine_n - 10) / 2;
}
