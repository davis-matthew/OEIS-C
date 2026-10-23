/*
 * A040193  Continued fraction for sqrt(208).
 * https://oeis.org/A040193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 14; for n > 0, a(n) = period[(n-1) mod 6] with period =
 *            [2,2,1,2,2,28]
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[6] = { (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)28 };
    if (n == 0u)
    {
        return (seq_t)14;
    }
    else
    {
        return period[(n - 1u) % 6u];
    }
}
