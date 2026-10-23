/*
 * A040929  Continued fraction for sqrt(960).
 * https://oeis.org/A040929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040929: Continued fraction for sqrt(960)
   Definition: a(0) = 30; for n ≥ 1, a(n) = 60 if n is even, otherwise 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)30;
    /* n is >0 at this point */
    if ((n & 1) == 0) return (seq_t)60;
    return (seq_t)1;
}
