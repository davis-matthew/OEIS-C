/*
 * A040789  Continued fraction for sqrt(818).
 * https://oeis.org/A040789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   A040789 - continued fraction for sqrt(818) a(0) = 28 a(n) = 56
 *            if n mod 5 == 0 and n > 0 a(n) = 1 otherwise
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* <external linkage> function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;

    if ((n % 5) == 0)
        return (seq_t)56;
    else
        return (seq_t)1;
}
