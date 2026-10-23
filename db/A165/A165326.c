/*
 * A165326  a(0)=a(1)=1, a(n) = -a(n-1) for n > 1.
 * https://oeis.org/A165326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(0)=1, a(1)=1, a(n) = -a(n-1) for n>1 (alternating 1 and -1)
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    /* For n ≥ 2 the sequence alternates: n even → -1, n odd → 1. */
    return (n & 1) ? (seq_t)1 : (seq_t)-1;
}
