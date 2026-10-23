/*
 * A040793  Continued fraction for sqrt(822).
 * https://oeis.org/A040793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 28; for n > 0: a(n) = 56 if 2|n and 3|n, a(n) = 2 if 2|n
 *            and 3∤n, a(n) = 28 if 3|n and 2∤n, a(n) = 1 otherwise
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 0 special case */
        return (seq_t)28;

    if ((n % 2 == 0) && (n % 3 == 0))
        return (seq_t)56;
    if (n % 2 == 0)
        return (seq_t)2;
    if (n % 3 == 0)
        return (seq_t)28;
    return (seq_t)1;
}
