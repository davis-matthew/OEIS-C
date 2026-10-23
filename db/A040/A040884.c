/*
 * A040884  Continued fraction for sqrt(915).
 * https://oeis.org/A040884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(915)
   a(0) = 30
   a(1) = 4
   a(2) = 60
   a(n) for n >= 3 repeats the pattern 4, 60, 4, 60, ...
   Formula: a(0)=30; for n>0, a(n) = (n is odd ? 4 : 60) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)30;
    /* For odd indices return 4, for even indices return 60 */
    return ((n & 1) ? (seq_t)4 : (seq_t)60);
}
