/*
 * A146951  Numbers that are congruent to 0 or 6 mod 10.
 * https://oeis.org/A146951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n + (n mod 2)
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This is equivalent to a(n) = 1/2 - (-1)^n/2 + 5*n from the OEIS entry. */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n * 5;
    /* Add 1 only when n is odd. */
    res += (seq_t)(n & 1);
    return res;
}
