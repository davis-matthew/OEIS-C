/*
 * A042964  Numbers that are congruent to 2 or 3 mod 4.
 * https://oeis.org/A042964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n if n is odd, otherwise a(n) = 2*n - 1
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This yields numbers congruent to 2 or 3 mod 4, with a(1)=2. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1ULL) == 0)     /* n is even */
        return nn * 2 - 1;
    else                     /* n is odd */
        return nn * 2;
}
