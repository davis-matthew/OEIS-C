/*
 * A137797  a(n) = 2*( (n+1) mod 5 ) - 2*( (n+1) mod 2 ).
 * https://oeis.org/A137797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*( (n+1) mod 5 ) - 2*( (n+1) mod 2 )
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (n+1) mod 5 and (n+1) mod 2 safely */
    seq_t mod5 = (seq_t)((n + 1) % 5);
    seq_t mod2 = (seq_t)((n + 1) % 2);
    return 2 * mod5 - 2 * mod2;
}
