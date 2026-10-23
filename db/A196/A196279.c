/*
 * A196279  Let r= (7n) mod 10 and x=floor(7n/10) be the last digit and
 *          leading part of 7n. Then a(n) = (x-2r)/7.
 * https://oeis.org/A196279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = (x - 2r) / 7 where r = (7*n) mod 10 and x = floor(7*n /
 *            10)
 * Range:     i8 n <= 18, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 7*n */
    seq_t prod = (seq_t)n * 7;

    /* last digit r and leading part x of 7*n */
    seq_t r = prod % 10;
    seq_t x = prod / 10;

    /* return (x - 2r)/7 */
    return (x - 2 * r) / 7;
}
