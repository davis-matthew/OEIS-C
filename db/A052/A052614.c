/*
 * A052614  E.g.f. 1/((1-x)(1-x^4)).
 * https://oeis.org/A052614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * (⌊n/4⌋ + 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 48, ... */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;          /* factorial n!  */

    seq_t multiplier = (seq_t)(n / 4 + 1);
    return fact * multiplier;     /* n! * (floor(n/4)+1) */
}
