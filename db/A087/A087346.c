/*
 * A087346  a(n) = A088116(100+n).
 * https://oeis.org/A087346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 11*(n mod 10) + 2*((n/10) mod 10)*(10 + (n mod 10))
 * Range:     u8 n <= 57, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 100 to access the last two decimal digits. */
    idx_t m = n % 100;
    idx_t t = m / 10;     /* tens digit of the last two digits */
    idx_t u = m % 10;     /* units digit of the last two digits */

    /* Compute the value in the same type as the sequence values. */
    seq_t res = 11 * (seq_t)u + 2 * (seq_t)t * (10 + (seq_t)u);
    return res;
}
